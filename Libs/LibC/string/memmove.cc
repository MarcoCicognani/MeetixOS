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

extern "C" void* memmove(void* dest, const void* src, usize num) {
    if ( (reinterpret_cast<usize>(dest) - reinterpret_cast<usize>(src)) >= num )
        return memcpy(dest, src, num);

    auto dst_byte = reinterpret_cast<u8*>(dest);
    auto src_byte = reinterpret_cast<const u8*>(src);
    for ( dst_byte += num, src_byte += num; num--; )
        *--dst_byte = *--src_byte;
    return dest;
}
