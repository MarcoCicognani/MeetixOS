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

#include <LibTC/Collection/Vector.hh>
#include <LibTC/Memory/Raw.hh>
#include <malloc.h>
#include <string.h>

namespace Details {

void const* bitwise_memmem(void const* haystack, usize haystack_len, void const* needle, usize needle_len) {
    u64            lookup   = 0xfffffffe;
    constexpr auto mask_len = static_cast<usize>(static_cast<u8>(-1)) + 1;

    u64 needle_mask[mask_len];
    for ( auto& mask : needle_mask )
        mask = 0xffffffff;

    for ( usize i = 0; i < needle_len; ++i )
        needle_mask[static_cast<u8 const*>(needle)[i]] &= ~(0x00000001 << i);

    for ( usize i = 0; i < haystack_len; ++i ) {
        lookup |= needle_mask[static_cast<u8 const*>(haystack)[i]];
        lookup <<= 1;

        if ( (lookup & (0x00000001 << needle_len)) == 0 )
            return static_cast<u8 const*>(haystack) + i - needle_len + 1;
    }
    return nullptr;
}

void const* kmp_memmem(u8 const* haystack, usize haystack_len, u8 const* needle, usize needle_len) {
    auto prepare_kmp_partial_table = [&] {
        TC::Vector<int> partial_table{};
        partial_table.resize(needle_len);

        usize position   = 1;
        int   candidate  = 0;
        partial_table[0] = -1;
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

    auto  partial_table          = prepare_kmp_partial_table();
    usize current_haystack_index = 0;
    int   needle_index           = 0;

    while ( current_haystack_index < haystack_len ) {
        if ( needle[needle_index] == haystack[current_haystack_index] ) {
            ++needle_index;
            ++current_haystack_index;
            if ( static_cast<usize>(needle_index) == needle_len )
                return reinterpret_cast<void const*>(haystack + needle_index);
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

} // namespace Details

extern "C" {

void* memcpy(void* dest, const void* src, usize num) {
    auto dest_ptr = dest;
    asm volatile("rep movsb" : "+D"(dest), "+S"(src), "+c"(num)::"memory");
    return dest_ptr;
}

void* memmove(void* dest, const void* src, usize num) {
    if ( (reinterpret_cast<usize>(dest) - reinterpret_cast<usize>(src)) >= num )
        return memcpy(dest, src, num);

    auto dst_byte = reinterpret_cast<u8*>(dest);
    auto src_byte = reinterpret_cast<const u8*>(src);
    for ( dst_byte += num, src_byte += num; num--; )
        *--dst_byte = *--src_byte;
    return dest;
}

void* memset(void* mem, int value, usize len) {
    auto dest = reinterpret_cast<usize>(mem);
    if ( !(dest & 0x3) && len >= 12 ) {
        auto usize_len   = len / sizeof(usize);
        auto byte_value  = static_cast<u8>(value);
        auto usize_value = byte_value << 24 | byte_value << 16 | byte_value << 8 | byte_value;

        asm volatile("rep stosl\n" : "=D"(dest) : "D"(dest), "c"(usize_len), "a"(usize_value) : "memory");

        len -= usize_len * sizeof(usize);
        if ( !len )
            return mem;
    }
    asm volatile("rep stosb\n" : "=D"(dest), "=c"(len) : "0"(dest), "1"(len), "a"(value) : "memory");
    return mem;
}

int memcmp(const void* mem_a, const void* mem_b, usize len) {
    auto byte_a = reinterpret_cast<const u8*>(mem_a);
    auto byte_b = reinterpret_cast<const u8*>(mem_b);

    for ( auto i = 0; i < len; i++ ) {
        if ( byte_a[i] > byte_b[i] )
            return 1;
        if ( byte_a[i] < byte_b[i] )
            return -1;
    }
    return 0;
}

void* memchr(const void* mem, int value, usize num) {
    auto byte = reinterpret_cast<const u8*>(mem);
    while ( num-- ) {
        if ( *byte == static_cast<u8>(value) )
            return reinterpret_cast<void*>(const_cast<u8*>(byte));
        ++byte;
    }
    return nullptr;
}

const void* memmem(const void* haystack, usize haystack_len, const void* needle, usize needle_len) {
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
        return Details::bitwise_memmem(haystack, haystack_len, needle, needle_len);
    else
        return Details::kmp_memmem(reinterpret_cast<u8 const*>(haystack),
                                   haystack_len,
                                   reinterpret_cast<u8 const*>(needle),
                                   needle_len);
}

char* strcpy(char* dest, const char* src) {
    char* begin_dest = dest;
    while ( (*dest++ = *src++) != '\0' )
        ;
    return begin_dest;
}

char* strncpy(char* dest, const char* src, usize num) {
    usize i;
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

char* strncat(char* dest, const char* src, usize n) {
    auto d = dest + strlen(dest);
    for ( ; n-- && *src; )
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

int strncmp(const char* str_a, const char* str_b, usize max) {
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

usize strxfrm(char* dest, const char* src, usize num) {
    usize i;
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

usize strcspn(const char* str, const char* chars) {
    auto ret = 0;
    while ( *str ) {
        if ( strchr(chars, *str) )
            return ret;
        ++str;
        ++ret;
    }
    return ret;
}

usize strspn(const char* str, const char* chars) {
    auto ret = 0;
    while ( *str && strchr(chars, *str++) )
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
        usize len = strlen(needle);
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

    usize token_start = 0;
    usize token_end   = 0;
    usize str_len     = strlen(str);
    usize delim_len   = strlen(delim);
    for ( usize i = 0; i < str_len; ++i ) {
        bool is_proper_delim = false;

        for ( usize j = 0; j < delim_len; ++j ) {
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

usize strlen(const char* str) {
    usize len = 0;
    while ( *(str++) )
        ++len;
    return len;
}

char* strdup(const char* str) {
    auto len              = strlen(str);
    auto error_or_new_str = TC::Memory::Raw::clean_alloc_array<char>(len + 1);
    if ( error_or_new_str.is_error() )
        return nullptr;

    auto new_str = error_or_new_str.unwrap_value();
    strncpy(new_str, str, len);
    return new_str;
}

char* strerror(int) {
    return const_cast<char*>("ERROR");
}
}