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

char* strncat(char* dest, const char* src, usize n) {
    char* d = dest + strlen(dest);
    for ( ; n-- && *src; )
        *d++ = *src++;

    *d = '\0';
    return dest;
}
