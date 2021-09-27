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

int strcmp(const char* str_a, const char* str_b) {
    const u8* mem_a8 = (const u8*)str_a;
    const u8* mem_b8 = (const u8*)str_b;

    for ( ;; ++mem_a8, ++mem_b8 ) {
        u8 a = *mem_a8;
        u8 b = *mem_b8;

        if ( a == 0 && b == 0 )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
}
