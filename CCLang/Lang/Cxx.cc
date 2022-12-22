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
#pragma ide diagnostic   ignored "bugprone-easily-swappable-parameters"
#pragma ide diagnostic   ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"
#pragma ide diagnostic   ignored "cppcoreguidelines-pro-type-cstyle-cast"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"
extern "C" {

void* memcpy(void* dest_ptr, const void* src_ptr, unsigned int n) {
    void* original_dest = dest_ptr;
    asm volatile("rep movsb" : "+D"(dest_ptr), "+S"(src_ptr), "+c"(n)::"memory");
    return original_dest;
}

void* memmove(void* dest_ptr, void const* src_ptr, unsigned int n) {
    if ( ((unsigned int)dest_ptr - (unsigned int)src_ptr) >= n ) {
        return memcpy(dest_ptr, src_ptr, n);
    }

    auto byte_dest = (unsigned char*)dest_ptr;
    auto byte_src  = (unsigned char const*)src_ptr;
    for ( byte_dest += n, byte_src += n; n--; ) {
        *--byte_dest = *--byte_src;
    }
    return dest_ptr;
}

void* memset(void* dest_ptr, int c, unsigned int n) {
    auto uint_dest = (unsigned int)dest_ptr;
    if ( !(uint_dest & 0x3) && n >= 12 ) {
        unsigned int const count  = n / sizeof(unsigned int);
        unsigned int const wide_c = c << 24 | c << 16 | c << 8 | c;
        asm volatile("rep stosl" : "=D"(uint_dest) : "D"(uint_dest), "c"(count), "a"(wide_c) : "memory");
        n -= count * sizeof(unsigned int);
        if ( n == 0u ) {
            return dest_ptr;
        }
    }
    asm volatile("rep stosb" : "=D"(uint_dest), "=c"(n) : "0"(uint_dest), "1"(n), "a"(c) : "memory");
    return dest_ptr;
}

int memcmp(const void* lhs, const void* rhs, unsigned int count) {
    auto byte_lhs = (unsigned char const*)lhs;
    auto byte_rhs = (unsigned char const*)rhs;
    while ( count-- > 0 ) {
        if ( *byte_lhs++ != *byte_rhs++ ) {
            return byte_lhs[-1] < byte_rhs[-1] ? -1 : 1;
        }
    }
    return 0;
}

unsigned int strlen(char const* str) {
    unsigned int len = 0u;
    while ( *(str++) != '\0' ) {
        ++len;
    }
    return len;
}

} /* extern "C" */
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

auto memcmp(void const* a, void const* b, usize count) -> Order {
    return bit_cast<Order>(::memcmp(a, b, count.unwrap()));
}

auto strlen(char const* c_str) -> usize {
    return ::strlen(c_str);
}

} /* namespace Cxx */