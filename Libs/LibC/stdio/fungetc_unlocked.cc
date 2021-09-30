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

#include <cerrno>
#include <cstdio>
#include <cstring>

int fungetc_unlocked(int c, FILE* stream) {
    /* if necessary, initialize stream buffer */
    if ( !(stream->m_flags & FILE_FLAG_BUFFER_SET) ) {
        if ( setdefbuf_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* unbuffered streams currently don't support this */
    if ( stream->m_buffer_mode == _IONBF ) {
        errno = EBADF;
        stream->m_flags |= FILE_FLAG_ERROR;
        return EOF;
    }

    /* if the last access was write, flush it */
    if ( stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_WRITE ) {
        if ( fflush_write_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* set stream flags */
    stream->m_flags |= FILE_FLAG_BUFFER_DIRECTION_READ;
    stream->m_flags &= ~FILE_FLAG_EOF;
    if ( c == EOF )
        return EOF;

    /* move the bytes if reached the limit */
    auto byte = static_cast<u8>(c);
    if ( !stream->m_buffered_bytes_read_offset ) {
        usize buffered_bytes = stream->m_buffered_bytes_read - stream->m_buffered_bytes_read_offset;
        usize buffer_space   = stream->m_io_buffer_len - buffered_bytes;
        if ( !buffer_space )
            return EOF;

        /* move bytes to the right */
        memmove(stream->m_io_buffer + buffer_space,
                stream->m_io_buffer,
                sizeof(stream->m_io_buffer[0]) * buffered_bytes);
        stream->m_buffered_bytes_read_offset = buffer_space;
        stream->m_buffered_bytes_read        = buffer_space + buffered_bytes;
    }

    /* store the byte into the buffer */
    stream->m_io_buffer[--stream->m_buffered_bytes_read_offset] = byte;
    return byte;
}
