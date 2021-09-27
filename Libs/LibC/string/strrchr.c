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

#include <stdbool.h>
#include <string.h>

char* strrchr(const char* str, int c) {
    const char* rev_str = str + strlen(str);

    while ( true ) {
        if ( *rev_str == c )
            return (char*)rev_str;
        if ( str == rev_str )
            break;

        --rev_str;
    }
    return NULL;
}
