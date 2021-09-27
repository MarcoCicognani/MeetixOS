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

#include <malloc.h>
#include <stdio.h>

int setvbuf_unlocked(FILE* stream, char* buffer, int mode, usize size) {
    /* free old buffer if library is owner */
    if ( (stream->m_buffer_mode != _IONBF) && (stream->m_io_buffer != NULL)
         && (stream->m_flags & FILE_FLAG_BUFFER_OWNER_LIBRARY) ) {
        free(stream->m_io_buffer);
    }

    /* create buffer if necessary */
    if ( !buffer ) {
        /* normalize buffer size */
        if ( !size ) {
            size = BUFSIZ;
        } else if ( size < BUFSIZMIN ) {
            size = BUFSIZMIN;
        }

        /* allocate buffer memory */
        buffer = (char*)malloc(size);
        if ( !buffer )
            return -1;

        /* set library as buffer owner */
        stream->m_flags |= FILE_FLAG_BUFFER_OWNER_LIBRARY;
    } else {
        /* set user as buffer owner */
        stream->m_flags &= ~FILE_FLAG_BUFFER_OWNER_LIBRARY;
    }

    /* set fields */
    stream->m_io_buffer                  = (u8*)buffer;
    stream->m_buffer_mode                = mode;
    stream->m_io_buffer_len              = size;
    stream->m_buffered_bytes_read        = 0;
    stream->m_buffered_bytes_read_offset = 0;
    stream->m_buffered_bytes_write       = 0;
    stream->m_flags |= FILE_FLAG_BUFFER_SET;
    return 0;
}
