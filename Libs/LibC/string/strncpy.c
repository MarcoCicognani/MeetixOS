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

char* strncpy(char* dest, const char* src, usize num) {
    usize i = 0;
    while ( i < num && src[i] != '\0' ) {
        dest[i] = src[i];
        ++i;
    }

    while ( i < num ) {
        dest[i] = '\0';
        ++i;
    }
    return dest;
}
