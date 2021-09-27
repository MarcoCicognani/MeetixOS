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

FILE* fopen(const char* filename, const char* mode) {
    /* allocate the stream structure */
    FILE* stream = (FILE*)calloc(sizeof(FILE), 1);
    if ( !stream ) {
        errno = ENOMEM;
        return NULL;
    }

    /* static-open it */
    FILE* res = fopen_static(filename, mode, stream);
    if ( !res ) {
        free(stream);
        return NULL;
    }
    return res;
}
