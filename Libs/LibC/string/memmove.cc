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
        auto src_byte = reinterpret_cast<const u8*>(src) + num - 1;
        auto dst_byte = reinterpret_cast<u8*>(dest) + num - 1;
        while ( num-- )
            *dst_byte-- = *src_byte--;
    } else {
        auto src_byte = reinterpret_cast<const u8*>(src);
        auto dst_byte = reinterpret_cast<u8*>(dest);
        while ( num-- )
            *dst_byte++ = *src_byte++;
    }
    return dest;
}
