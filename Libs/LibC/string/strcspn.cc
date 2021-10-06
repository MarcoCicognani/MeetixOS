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

extern "C" usize strcspn(const char* str, const char* chars) {
    auto ret = 0;
    while ( *str ) {
        if ( strchr(chars, *str) )
            return ret;
        ++str;
        ++ret;
    }
    return ret;
}
