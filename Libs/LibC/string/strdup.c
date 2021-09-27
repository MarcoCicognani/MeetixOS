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

#include <malloc.h>
#include <string.h>

char* strdup(const char* src) {
    char* dest = (char*)malloc(strlen(src) + 1);
    if ( !dest )
        return NULL;

    strcpy(dest, src);
    return dest;
}
