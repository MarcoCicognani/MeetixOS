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

extern "C" void* memcpy(void* dest, const void* src, usize num) {
    auto dest_ptr = dest;
    asm volatile("rep movsb" : "+D"(dest), "+S"(src), "+c"(num)::"memory");
    return dest_ptr;
}
