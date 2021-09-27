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

#include "stdio_internal.h"

#include <stdio.h>

int fprintf(FILE* stream, const char* format, ...) {
    va_list va;
    va_start(va, format);
    int res = vfprintf(stream, format, va);
    va_end(va);
    return res;
}
