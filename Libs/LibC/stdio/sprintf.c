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

int sprintf(char* s, const char* format, ...) {
    va_list va;
    va_start(va, format);
    int res = vsprintf(s, format, va);
    va_end(va);
    return res;
}
