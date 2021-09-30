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

int setdefbuf_unlocked(FILE* stream) {
    /* try to set the buffer */
    if ( setvbuf_unlocked(stream, 0, _IOFBF, BUFSIZ) == 0 )
        return 0;

    /* failed, try to make unbuffered */
    if ( setvbuf_unlocked(stream, 0, _IONBF, BUFSIZ) == 0 )
        return 0;

    return EOF;
}
