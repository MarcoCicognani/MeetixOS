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

extern "C" int tolower(int c) {
    if ( c >= 'A' && c <= 'Z' ) {
        return 'a' + (c - 'A');
    }
    return c;
}
