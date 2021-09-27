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

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

int atoi(const char* str) {
    long long_int = strtol(str, (char**)NULL, 10);

    if ( long_int > INT_MAX ) {
        errno = ERANGE;
        return INT_MAX;
    }

    if ( long_int < INT_MIN ) {
        errno = ERANGE;
        return INT_MIN;
    }

    return (int)long_int;
}
