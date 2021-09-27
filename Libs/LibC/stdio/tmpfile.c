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

#include <errno.h>
#include <malloc.h>
#include <stdio.h>

FILE* tmpfile() {
    /* create the buffer for the name */
    char* name_buf = (char*)malloc(L_tmpnam);
    if ( !name_buf ) {
        errno = ENOMEM;
        return NULL;
    }

    /* obtain temporary filename */
    char* name = tmpnam(name_buf);
    if ( !name ) {
        free(name_buf);
        return NULL;
    }

    /* create the file and return the stream */
    FILE* stream = fopen(name, "w+");
    free(name_buf);
    return stream;
}
