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

#include <cstring>

void* memcpy(void* dest, const void* src, usize num) {
    auto dst_byte = reinterpret_cast<u8*>(dest);
    auto src_byte = reinterpret_cast<const u8*>(src);

    while ( num-- )
        *dst_byte++ = *src_byte++;
    return dest;
}
