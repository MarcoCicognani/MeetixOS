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

extern "C" int strncmp(const char* str_a, const char* str_b, usize max) {
    for ( ; max--; ++str_a, ++str_b ) {
        auto a = *str_a;
        auto b = *str_b;

        if ( !a && !b )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
    return 0;
}
