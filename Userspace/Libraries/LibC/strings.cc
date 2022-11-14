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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"

#include <LibC/ctype.h>
#include <LibC/strings.h>

extern "C" {

int strcasecmp(const char* str_a, const char* str_b) {
    for ( ;; ++str_a, ++str_b ) {
        auto a = tolower(*str_a);
        auto b = tolower(*str_b);

        if ( a == 0 && b == 0 )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
}

int strncasecmp(const char* str_a, const char* str_b, size_t max) {
    for ( ; max--; ++str_a, ++str_b ) {
        auto a = tolower(*str_a);
        auto b = tolower(*str_b);

        if ( a == 0 && b == 0 )
            return 0;
        else if ( a < b )
            return -1;
        else if ( a > b )
            return 1;
    }
    return 0;
}

} /* extern "C" */

#pragma clang diagnostic pop
