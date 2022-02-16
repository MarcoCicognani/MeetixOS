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

int strcmp(const char* str_a, const char* str_b) {
    for ( ;; ++str_a, ++str_b ) {
        auto a = *str_a;
        auto b = *str_b;

        if ( !a && !b )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
}
