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

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

extern "C" int atoi(const char* str) {
    auto long_int = strtol(str, nullptr, 10);

    if ( long_int > INT_MAX ) {
        errno = ERANGE;
        return INT_MAX;
    }
    if ( long_int < INT_MIN ) {
        errno = ERANGE;
        return INT_MIN;
    }

    return static_cast<int>(long_int);
}
