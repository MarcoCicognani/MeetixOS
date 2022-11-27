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

#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"
#pragma ide diagnostic   ignored "bugprone-easily-swappable-parameters"
extern "C" {

void* memcpy(void* dest_ptr, const void* src_ptr, __SIZE_TYPE__ n) {
    void* original_dest = dest_ptr;
    asm volatile("rep movsb" : "+D"(dest_ptr), "+S"(src_ptr), "+c"(n)::"memory");
    return original_dest;
}


void* memmove(void* dest_ptr, void const* src_ptr, __SIZE_TYPE__ n) {
    if ( (Cxx::bit_cast<__SIZE_TYPE__>(dest_ptr) - Cxx::bit_cast<__SIZE_TYPE__>(src_ptr)) >= n )
        return memcpy(dest_ptr, src_ptr, n);

    auto byte_dest = Cxx::bit_cast<__UINT8_TYPE__*>(dest_ptr);
    auto byte_src  = Cxx::bit_cast<__UINT8_TYPE__ const*>(src_ptr);
    for ( byte_dest += n, byte_src += n; n--; )
        *--byte_dest = *--byte_src;
    return dest_ptr;
}

void* memset(void* dest_ptr, int c, __SIZE_TYPE__ n) {
    auto dest = Cxx::bit_cast<__SIZE_TYPE__>(dest_ptr);
    if ( !(dest & 0x3) && n >= 12 ) {
        __SIZE_TYPE__ count  = n / sizeof(__SIZE_TYPE__);
        __SIZE_TYPE__ wide_c = c << 24 | c << 16 | c << 8 | c;
        asm volatile("rep stosl" : "=D"(dest) : "D"(dest), "c"(count), "a"(wide_c) : "memory");
        n -= count * sizeof(__SIZE_TYPE__);
        if ( n == 0 )
            return dest_ptr;
    }
    asm volatile("rep stosb" : "=D"(dest), "=c"(n) : "0"(dest), "1"(n), "a"(c) : "memory");
    return dest_ptr;
}


__SIZE_TYPE__ strlen(char const* str) {
    __SIZE_TYPE__ len = 0;
    while ( *(str++) != '\0' )
        ++len;
    return len;
}

} /* extern "C" */
#pragma clang diagnostic pop
#pragma clang diagnostic pop

namespace Cxx {

auto memcpy(void* dest_ptr, void const* src_ptr, usize count) -> void* {
    return ::memcpy(dest_ptr, src_ptr, count.unwrap());
}

auto memmove(void* dest_ptr, void const* src_ptr, usize count) -> void* {
    return ::memmove(dest_ptr, src_ptr, count.unwrap());
}

auto memset(void* dest_ptr, int c, usize count) -> void* {
    return ::memset(dest_ptr, c, count.unwrap());
}

auto strlen(char const* c_str) -> usize {
    return ::strlen(c_str);
}

} /* namespace Cxx */