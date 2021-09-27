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

#include <ctype.h>

int toupper(int c) {
    if ( c >= 'a' && c <= 'z' ) {
        return 'A' + (c - 'a');
    }
    return c;
}
