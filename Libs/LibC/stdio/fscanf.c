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

int fscanf(A_UNUSED FILE* stream, A_UNUSED const char* format, ...) {
    __NOT_IMPLEMENTED(fscanf);
    return -1;
}
