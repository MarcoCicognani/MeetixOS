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

void* memory_copy(void* dest, const void* src, unsigned int len) {
    auto byte_dest = reinterpret_cast<unsigned char*>(dest);
    auto byte_src  = reinterpret_cast<const unsigned char*>(src);
    while ( len-- )
        *byte_dest++ = *byte_src++;
    return dest;
}

unsigned int string_len(const char* str) {
    auto len = 0;
    while ( *str++ )
        ++len;
    return len;
}
