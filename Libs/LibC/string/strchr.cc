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

extern "C" char* strchr(const char* str, int c) {
    while ( *str ) {
        if ( *str == c )
            return const_cast<char*>(str);
        ++str;
    }
    return nullptr;
}
