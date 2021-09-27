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

void* memcpy(void* dest, const void* src, usize num) {
    u8*       dest_8 = (u8*)dest;
    const u8* src_8  = (const u8*)src;
    while ( num-- )
        *dest_8++ = *src_8++;

    return dest;
}
