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

char* strpbrk(const char* str_a, const char* str_b) {
    while ( *str_a ) {
        const char* p = str_b;
        while ( *p ) {
            if ( *str_a == *p ) {
                return (char*)str_a;
            }
            ++p;
        }

        ++str_a;
    }
    return NULL;
}
