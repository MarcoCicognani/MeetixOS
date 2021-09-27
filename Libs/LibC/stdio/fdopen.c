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

#include <errno.h>
#include <malloc.h>
#include <stdio.h>

FILE* fdopen(int fd, const char* mode) {
    /* construct the stream */
    FILE* stream = (FILE*)calloc(sizeof(FILE), 1);
    if ( !stream ) {
        errno = ENOMEM;
        return NULL;
    }

    /* fill the stream */
    if ( fdopen_static(fd, mode, stream) == EOF )
        return NULL;
    return stream;
}
