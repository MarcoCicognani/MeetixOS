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

char* strchr(const char* str, int c) {
    while ( *str ) {
        if ( *str == c )
            return (char*)str;
        ++str;
    }
    return NULL;
}
