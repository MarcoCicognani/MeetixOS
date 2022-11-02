/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibC/malloc.h>
#include <LibC/string.h>
#include <LibTC/Alloc/Details.hh>
#include <LibTC/Alloc/Vector.hh>
#include <LibTC/Lang/Cxx.hh>
#include <LibTC/Lang/Range.hh>

namespace Details {

auto bitwise_memmem(void const* haystack, size_t haystack_len, void const* needle, size_t needle_len) -> void const* {
    constexpr auto mask_len = static_cast<size_t>(static_cast<uint8_t>(-1)) + 1;

    uint32_t lookup = 0xfffffffe;
    uint32_t needle_mask[mask_len];
    for ( auto& mask : needle_mask )
        mask = 0xffffffff;

    for ( auto const i : Range{ 0u, needle_len } )
        needle_mask[static_cast<uint8_t const*>(needle)[i]] &= ~(0x00000001 << i);

    for ( auto const i : Range{ 0u, haystack_len } ) {
        lookup |= needle_mask[static_cast<uint8_t const*>(haystack)[i]];
        lookup <<= 1;

        if ( (lookup & (0x00000001 << needle_len)) == 0 )
            return static_cast<uint8_t const*>(haystack) + i - needle_len + 1;
    }
    return nullptr;
}

auto kmp_memmem(uint8_t const* haystack, size_t haystack_len, uint8_t const* needle, size_t needle_len) -> void const* {
    auto try_prepare_kmp_partial_table = [&]() -> ErrorOr<Vector<int32_t>> {
        auto partial_table = try$(Vector<int32_t>::try_construct_with_capacity(needle_len));

        size_t  position  = 1;
        int32_t candidate = 0;
        partial_table[0]  = -1;
        while ( position < needle_len ) {
            if ( needle[position] == needle[candidate] )
                partial_table[position] = partial_table[candidate];
            else {
                partial_table[position] = candidate;
                do {
                    candidate = partial_table[candidate];
                } while ( candidate >= 0 && needle[candidate] != needle[position] );
            }

            ++position;
            ++candidate;
        }
        return partial_table;
    };

    /* prepare the KMP table */
    auto error_or_partial_table = try_prepare_kmp_partial_table();
    if ( error_or_partial_table.is_error() ) {
        errno = error_or_partial_table.unwrap_error().code();
        return nullptr;
    }

    auto const partial_table          = error_or_partial_table.unwrap();
    size_t     current_haystack_index = 0;
    int        needle_index           = 0;

    while ( current_haystack_index < haystack_len ) {
        if ( needle[needle_index] == haystack[current_haystack_index] ) {
            ++needle_index;
            ++current_haystack_index;
            if ( static_cast<size_t>(needle_index) == needle_len )
                return bit_cast<void const*>(haystack + needle_index);
            else
                continue;
        }

        needle_index = partial_table[needle_index];
        if ( needle_index < 0 ) {
            ++needle_index;
            ++current_haystack_index;
        }
    }
    return nullptr;
}

} /* namespace Details */

extern "C" {

void* memcpy(void* dest, const void* src, size_t num) {
    auto dest_ptr = dest;
    asm volatile("rep movsb" : "+D"(dest), "+S"(src), "+c"(num)::"memory");
    return dest_ptr;
}

void* memmove(void* dest, const void* src, size_t num) {
    if ( (bit_cast<size_t>(dest) - bit_cast<size_t>(src)) >= num )
        return memcpy(dest, src, num);

    auto dst_byte = bit_cast<uint8_t*>(dest);
    auto src_byte = bit_cast<uint8_t const*>(src);
    for ( dst_byte += num, src_byte += num; num--; )
        *--dst_byte = *--src_byte;
    return dest;
}

void* memset(void* mem, int value, size_t len) {
    auto dest = bit_cast<size_t>(mem);
    if ( !(dest & 0x3) && len >= 12 ) {
        auto ints_len       = len / sizeof(size_t);
        auto byte_value     = static_cast<uint8_t>(value);
        auto repeated_value = byte_value << 24 | byte_value << 16 | byte_value << 8 | byte_value;
        asm volatile("rep stosl\n" : "=D"(dest) : "D"(dest), "c"(ints_len), "a"(repeated_value) : "memory");

        len -= ints_len * sizeof(size_t);
        if ( !len )
            return mem;
    }
    asm volatile("rep stosb\n" : "=D"(dest), "=c"(len) : "0"(dest), "1"(len), "a"(value) : "memory");
    return mem;
}

int memcmp(const void* mem_a, const void* mem_b, size_t len) {
    auto byte_a = bit_cast<uint8_t const*>(mem_a);
    auto byte_b = bit_cast<uint8_t const*>(mem_b);

    for ( auto const i : Range{ 0u, len } ) {
        if ( byte_a[i] > byte_b[i] )
            return 1;
        if ( byte_a[i] < byte_b[i] )
            return -1;
    }
    return 0;
}

void* memchr(const void* mem, int value, size_t num) {
    auto byte = bit_cast<uint8_t const*>(mem);
    while ( num-- ) {
        if ( *byte == static_cast<uint8_t>(value) )
            return bit_cast<void*>(const_cast<uint8_t*>(byte));
        ++byte;
    }
    return nullptr;
}

const void* memmem(const void* haystack, size_t haystack_len, const void* needle, size_t needle_len) {
    if ( needle_len == 0 )
        return haystack;

    if ( haystack_len < needle_len )
        return nullptr;

    if ( haystack_len == needle_len ) {
        if ( memcmp(haystack, needle, haystack_len) == 0 )
            return haystack;
        else
            return nullptr;
    }

    if ( needle_len < 32 )
        return ::Details::bitwise_memmem(haystack, haystack_len, needle, needle_len);
    else
        return ::Details::kmp_memmem(bit_cast<uint8_t const*>(haystack), haystack_len, bit_cast<uint8_t const*>(needle), needle_len);
}

char* strcpy(char* dest, const char* src) {
    char* begin_dest = dest;
    while ( (*dest++ = *src++) != '\0' )
        ;
    return begin_dest;
}

char* strncpy(char* dest, const char* src, size_t num) {
    size_t i;
    for ( i = 0; i < num && src[i] != '\0'; ++i )
        dest[i] = src[i];
    for ( ; i < num; ++i )
        dest[i] = '\0';
    return dest;
}

char* strcat(char* dest, const char* src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}

char* strncat(char* dest, const char* src, size_t n) {
    auto d = dest + strlen(dest);
    while ( n-- && *src )
        *d++ = *src++;

    *d = '\0';
    return dest;
}

int strcmp(const char* str_a, const char* str_b) {
    for ( ;; ++str_a, ++str_b ) {
        auto a = *str_a;
        auto b = *str_b;

        if ( !a && !b )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
}

int strncmp(const char* str_a, const char* str_b, size_t max) {
    for ( ; max--; ++str_a, ++str_b ) {
        auto a = *str_a;
        auto b = *str_b;

        if ( !a && !b )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
    return 0;
}

int strcoll(const char* str_a, const char* str_b) {
    return strcmp(str_a, str_b);
}

size_t strxfrm(char* dest, const char* src, size_t num) {
    size_t i;
    for ( i = 0; i < num && src[i] != '\0'; ++i )
        dest[i] = src[i];
    for ( ; i < num; ++i )
        dest[i] = '\0';
    return i;
}

char* strchr(const char* str, int c) {
    while ( *str ) {
        if ( *str == c )
            return const_cast<char*>(str);
        ++str;
    }
    return nullptr;
}

char* strrchr(const char* str, int c) {
    auto rev_str = str + strlen(str);
    while ( true ) {
        if ( *rev_str == c )
            return const_cast<char*>(rev_str);
        if ( str == rev_str )
            break;

        --rev_str;
    }
    return nullptr;
}

size_t strcspn(const char* str, const char* chars) {
    auto ret = 0;
    while ( *str ) {
        if ( strchr(chars, *str) )
            return ret;
        ++str;
        ++ret;
    }
    return ret;
}

size_t strspn(const char* str, const char* chars) {
    auto ret = 0;
    while ( *str != '\0' && strchr(chars, *str++) )
        ++ret;
    return ret;
}

char* strpbrk(const char* str_a, const char* str_b) {
    while ( *str_a ) {
        auto p = str_b;
        while ( *p ) {
            if ( *str_a == *p )
                return const_cast<char*>(str_a);
            ++p;
        }
        ++str_a;
    }
    return nullptr;
}

char* strstr(const char* haystack, const char* needle) {
    char nch;
    char hch;
    if ( (nch = *needle++) != 0 ) {
        size_t len = strlen(needle);
        do {
            do {
                if ( (hch = *haystack++) == 0 )
                    return nullptr;
            } while ( hch != nch );
        } while ( strncmp(haystack, needle, len) != 0 );
        --haystack;
    }
    return const_cast<char*>(haystack);
}

char* strtok(char* str, const char* delim) {
    static char* saved_str;
    return strtok_r(str, delim, &saved_str);
}

char* strtok_r(char* str, const char* delim, char** saved_str) {
    if ( !str ) {
        if ( !saved_str )
            return nullptr;
        str = *saved_str;
    }

    size_t token_start = 0;
    size_t token_end   = 0;
    size_t str_len     = strlen(str);
    size_t delim_len   = strlen(delim);
    for ( size_t i = 0; i < str_len; ++i ) {
        bool is_proper_delim = false;

        for ( size_t j = 0; j < delim_len; ++j ) {
            if ( str[i] == delim[j] ) {
                // Skip beginning delimiters
                if ( token_end - token_start == 0 ) {
                    ++token_start;
                    break;
                }

                is_proper_delim = true;
            }
        }

        ++token_end;
        if ( is_proper_delim && token_end > 0 ) {
            --token_end;
            break;
        }
    }

    if ( str[token_start] == '\0' )
        return nullptr;

    if ( token_end == 0 ) {
        *saved_str = nullptr;
        return &str[token_start];
    }

    if ( str[token_end] == '\0' )
        *saved_str = &str[token_end];
    else
        *saved_str = &str[token_end + 1];

    str[token_end] = '\0';
    return &str[token_start];
}

size_t strlen(const char* str) {
    VERIFY_NOT_NULL(str);

    auto const str_start = str;
    while ( *(str++) )
        ;
    return str - str_start;
}

char* strdup(const char* str) {
    auto buffer_len          = strlen(str);
    auto error_or_new_buffer = TC::Raw::clean_alloc_array<char>(buffer_len + 1);
    if ( error_or_new_buffer.is_error() ) {
        errno = error_or_new_buffer.unwrap_error().code();
        return nullptr;
    }

    auto new_buffer = error_or_new_buffer.unwrap_value();
    strncpy(new_buffer, str, buffer_len);
    return new_buffer;
}

char* strerror(int error) {
    static constexpr char const* S_ERRNO_AS_STR[] = { "No error",
                                                      "Argument list too long",
                                                      "Permission denied",
                                                      "Address already in use",
                                                      "Can't assign requested address",
                                                      "Address family not supported",
                                                      "Resource unavailable, try again",
                                                      "Connection already in progress",
                                                      "Bad file descriptor",
                                                      "Bad message",
                                                      "Device or resource busy",
                                                      "Operation cancelled",
                                                      "No child processes",
                                                      "Connection aborted",
                                                      "Connection refused",
                                                      "Connection reset",
                                                      "Resource deadlock would occur",
                                                      "Destination address required",
                                                      "Mathematics argument out of domain of function",
                                                      "Reserved",
                                                      "File exists",
                                                      "Bad address",
                                                      "File too large",
                                                      "Host is unreachable",
                                                      "Identifier removed",
                                                      "Illegal byte sequence",
                                                      "Operation in progress",
                                                      "Interrupted function",
                                                      "Invalid argument",
                                                      "I/O error",
                                                      "Socket is connected",
                                                      "Is a directory",
                                                      "Too many levels of symbolic links",
                                                      "File descriptor value too large",
                                                      "Too many links",
                                                      "Message too large",
                                                      "Reserved",
                                                      "Filename too long",
                                                      "Network is down",
                                                      "Connection aborted by network",
                                                      "Network unreachable",
                                                      "Too many files open in system",
                                                      "No buffer space available",
                                                      "No such device",
                                                      "No such file or directory",
                                                      "Executable file format error",
                                                      "No locks available",
                                                      "Reserved",
                                                      "Not enough space",
                                                      "No message of the desired type",
                                                      "Protocol not available",
                                                      "No space left on device",
                                                      "Function not supported",
                                                      "The socket is not connected",
                                                      "Not a directory or a symbolic link to a directory",
                                                      "Directory not empty",
                                                      "State not recoverable",
                                                      "Not a socket",
                                                      "Not supported (maybe the same value as [EOPNOTSUPP])",
                                                      "Inappropriate I/O control operation",
                                                      "No such device or address",
                                                      "Operation not supported on socket (maybe the same value as ENOTSUP)",
                                                      "Value too large to be stored in data type",
                                                      "Previous owner died",
                                                      "Operation not permitted",
                                                      "Broken pipe",
                                                      "Protocol error",
                                                      "Protocol not supported",
                                                      "Protocol wrong type for socket",
                                                      "Result too large",
                                                      "Read-only file system",
                                                      "Invalid seek",
                                                      "No such process",
                                                      "Reserved",
                                                      "Connection timed out",
                                                      "Text file busy",
                                                      "Operation would block (maybe the same value as [EAGAIN])",
                                                      "Cross-device link",
                                                      "Custom error, used only by LibTC/Error when only string is provided" };

    if ( error > 0 && error < EMAX )
        return const_cast<char*>(S_ERRNO_AS_STR[error]);
    else {
        errno = EINVAL;
        return const_cast<char*>("Unknown error");
    }
}

} /* extern "C" */

#pragma clang diagnostic pop
