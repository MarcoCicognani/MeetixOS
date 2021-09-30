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

extern "C" int fclose(FILE* stream) {
    auto res = fclose_static(stream);
    if ( res != 0 )
        return EOF;

    /* destroy the stream */
    delete stream;
    return 0;
}
