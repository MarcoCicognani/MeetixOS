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

#include <string.h>

extern "C" int memcmp(const void* mem_a, const void* mem_b, usize len) {
    auto byte_a = reinterpret_cast<const u8*>(mem_a);
    auto byte_b = reinterpret_cast<const u8*>(mem_b);

    for ( auto i = 0; i < len; i++ ) {
        if ( byte_a[i] > byte_b[i] )
            return 1;
        if ( byte_a[i] < byte_b[i] )
            return -1;
    }
    return 0;
}
