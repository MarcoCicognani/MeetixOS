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

int printf(const char* format, ...) {
    va_list va;
    va_start(va, format);
    int res = vprintf(format, va);
    va_end(va);
    return res;
}
