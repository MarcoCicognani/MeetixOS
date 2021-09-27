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

void* memchr(const void* mem, int value, usize num) {
    const u8* byte = (u8*)mem;

    while ( num-- ) {
        if ( *byte == (u8)value )
            return (void*)byte;

        ++byte;
    }
    return NULL;
}
