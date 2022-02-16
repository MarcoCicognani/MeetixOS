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

#include "__internal.hh"

#include <Api/StdInt.h>

void* memory_copy(void* dest, const void* src, usize len) {
    auto byte_dest = reinterpret_cast<u8*>(dest);
    auto byte_src  = reinterpret_cast<const u8*>(src);
    while ( len-- )
        *byte_dest++ = *byte_src++;
    return dest;
}

usize string_len(const char* str) {
    auto len = 0;
    while ( *str++ )
        ++len;
    return len;
}
