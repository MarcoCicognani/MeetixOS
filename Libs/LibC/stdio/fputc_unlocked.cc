/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "stdio_internal.hh"

#include <stdio.h>

int fputc_unlocked(int c, FILE* stream) {
    auto byte = static_cast<u8>(c);

    /* if necessary, initialize stream buffer */
    if ( !(stream->m_flags & FILE_FLAG_BUFFER_SET) ) {
        if ( setdefbuf_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* for unbuffered streams, perform direct write */
    if ( stream->m_buffer_mode == _IONBF ) {
        if ( fwrite_unlocked(&byte, 1, 1, stream) != 1 ) {
            return EOF;
        }
        return byte;
    }

    /* if the last access was a read, flush it */
    if ( stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_READ ) {
        if ( fflush_read_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* check if buffer is full */
    if ( stream->m_buffered_bytes_write == stream->m_io_buffer_len ) {
        if ( fflush_write_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* put the byte into the buffer */
    stream->m_flags |= FILE_FLAG_BUFFER_DIRECTION_WRITE;
    stream->m_io_buffer[stream->m_buffered_bytes_write++] = byte;

    /* flush stream if its line-buffered and a newline occurs */
    if ( stream->m_buffer_mode == _IOLBF && byte == '\n' ) {
        if ( fflush_write_unlocked(stream) == EOF ) {
            return EOF;
        }
    }
    return byte;
}
