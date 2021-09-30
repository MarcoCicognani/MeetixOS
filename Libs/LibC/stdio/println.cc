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

#include <cstdio>

extern "C" int println(const char* format, ...) {
    va_list va;
    va_start(va, format);
    auto res = vprintf(format, va);
    va_end(va);

    if ( printf("\n") != 1 )
        return EOF;
    else
        return res;
}
