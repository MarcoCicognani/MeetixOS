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

int fflush_unlocked(FILE* stream) {
    /* flush read part of the buffer */
    if ( (stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_READ)
         && (fflush_read_unlocked(stream) == EOF) ) {
        return EOF;
    }

    /* flush write part of the buffer */
    if ( (stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_WRITE)
         && (fflush_write_unlocked(stream) == EOF) ) {
        return EOF;
    }
    return 0;
}
