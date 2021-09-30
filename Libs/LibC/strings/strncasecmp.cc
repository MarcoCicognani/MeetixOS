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

#include <cctype>
#include <strings.h>

extern "C" int strncasecmp(const char* str_a, const char* str_b, usize max) {
    for ( ; max--; ++str_a, ++str_b ) {
        auto a = tolower(*str_a);
        auto b = tolower(*str_b);

        if ( !a && !b )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
    return 0;
}
