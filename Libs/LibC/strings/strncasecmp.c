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

#include <ctype.h>
#include <strings.h>

int strncasecmp(const char* str_a, const char* str_b, usize max) {
    const u8* mem_a8 = (const u8*)str_a;
    const u8* mem_b8 = (const u8*)str_b;

    for ( ; max--; ++mem_a8, ++mem_b8 ) {
        u8 a = tolower(*mem_a8);
        u8 b = tolower(*mem_b8);

        if ( a == 0 && b == 0 ) {
            return 0;
        } else if ( a < b ) {
            return -1;
        } else if ( a > b ) {
            return 1;
        }
    }
    return 0;
}
