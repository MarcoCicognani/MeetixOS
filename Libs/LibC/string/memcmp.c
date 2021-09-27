/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <string.h>

int memcmp(const void* mem_a, const void* mem_b, usize len) {
    const u8* mem_a8 = (const u8*)mem_a;
    const u8* mem_b8 = (const u8*)mem_b;

    for ( usize i = 0; i < len; i++ ) {
        if ( mem_a8[i] > mem_b8[i] )
            return 1;
        if ( mem_a8[i] < mem_b8[i] )
            return -1;
    }
    return 0;
}
