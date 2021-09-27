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

void* memmove(void* dest, const void* src, usize num) {
    if ( dest > src ) {
        const u8* src_8  = ((const u8*)src) + num - 1;
        u8*       dest_8 = ((u8*)dest) + num - 1;
        while ( num-- )
            *dest_8-- = *src_8--;
    } else {
        const u8* src_8  = (const u8*)src;
        u8*       dest_8 = (u8*)dest;
        while ( num-- )
            *dest_8++ = *src_8++;
    }
    return dest;
}
