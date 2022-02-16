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

#include <string.h>

extern "C" void* memset(void* mem, int value, usize len) {
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
