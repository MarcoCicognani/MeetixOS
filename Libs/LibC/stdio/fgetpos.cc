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

#include "stdio_internal.hh"

#include <stdio.h>

extern "C" int fgetpos(FILE* stream, fpos_t* pos) {
    if ( (*pos = ftello(stream)) < 0 )
        return -1;
    else
        return 0;
}
