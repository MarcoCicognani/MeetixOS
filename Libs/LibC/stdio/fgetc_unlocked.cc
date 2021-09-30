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

int fgetc_unlocked(FILE* stream) {
    /* if necessary, initialize stream buffer */
    if ( !(stream->m_flags & FILE_FLAG_BUFFER_SET) ) {
        if ( setdefbuf_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* for unbuffered streams, perform direct write */
    if ( stream->m_buffer_mode == _IONBF ) {
        u8 byte;
        if ( fread_unlocked(&byte, 1, 1, stream) != 1 ) {
            return EOF;
        }
        return byte;
    }

    /* if the last access was write, flush it */
    if ( stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_WRITE ) {
        if ( fflush_write_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* set direction */
    stream->m_flags |= FILE_FLAG_BUFFER_DIRECTION_READ;

    /* store into buffer the bytes */
    if ( stream->m_buffered_bytes_read_offset >= stream->m_buffered_bytes_read ) {
        /* keep some space for ungetc-calls to avoid moving memory */
        auto unget_space = FILE_UNGET_PRESERVED_SPACE;

        /* if buffer is too small, leave no space */
        if ( unget_space >= stream->m_io_buffer_len )
            unget_space = 0;

        /* fill buffer with data from stream backend */
        auto read_bytes = stream->m_impl_read(stream->m_io_buffer + unget_space,
                                              stream->m_io_buffer_len - unget_space,
                                              stream);
        if ( !read_bytes ) {
            stream->m_flags |= FILE_FLAG_EOF;
            return EOF;

        } else if ( read_bytes < 0 ) {
            stream->m_flags |= FILE_FLAG_ERROR;
            return EOF;
        }

        /* set buffer fields */
        stream->m_buffered_bytes_read        = unget_space + read_bytes;
        stream->m_buffered_bytes_read_offset = unget_space;
    }

    /* return the next character */
    return stream->m_io_buffer[stream->m_buffered_bytes_read_offset++];
}
