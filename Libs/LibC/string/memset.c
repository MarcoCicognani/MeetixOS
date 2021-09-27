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

void* memset(void* mem, int value, usize len) {
    u8* byte = (u8*)mem;
    while ( len-- )
        byte[len] = (u8)value;

    return mem;
}
