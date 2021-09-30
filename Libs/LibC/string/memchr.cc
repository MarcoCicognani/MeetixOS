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

extern "C" void* memchr(const void* mem, int value, usize num) {
    auto byte = reinterpret_cast<const u8*>(mem);
    while ( num-- ) {
        if ( *byte == static_cast<u8>(value) )
            return reinterpret_cast<void*>(const_cast<u8*>(byte));
        ++byte;
    }
    return nullptr;
}
