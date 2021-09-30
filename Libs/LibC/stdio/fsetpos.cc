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

#include "stdio_internal.hh"

#include <stdio.h>

extern "C" int fsetpos(FILE* stream, const fpos_t* pos) {
    if ( fseeko(stream, *pos, SEEK_SET) < 0 )
        return -1;
    else
        return 0;
}
