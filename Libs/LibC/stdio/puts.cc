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

#include <stdio.h>

extern "C" int puts(const char* str) {
    auto ret = printf("%s\n", str);
    if ( ret < 0 )
        return EOF;
    return ret;
}
