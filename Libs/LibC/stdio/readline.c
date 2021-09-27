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
#include <stdio.h>

bool readline(FILE* stream, char* buffer, usize len) {
    if ( !stream || !buffer || !len ) {
        errno = EINVAL;
        return 0;
    }

    /* read the stream line-by-line */
    char  c;
    usize count = 0;
    while ( count < len && ((c = (char)fgetc(stream)) != '\n') && c != EOF )
        buffer[count++] = c;

    /* close the buffer */
    buffer[count] = '\0';
    return (c != EOF);
}