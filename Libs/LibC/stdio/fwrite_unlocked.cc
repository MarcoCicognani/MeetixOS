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

#include <errno.h>
#include <stdio.h>
#include <string.h>

usize fwrite_unlocked(const void* ptr, usize size, usize count, FILE* stream) {
    if ( !size || !count )
        return 0;

    /* check if stream is writable */
    if ( !(stream->m_flags & FILE_FLAG_MODE_WRITE) ) {
        errno = EBADF;
        stream->m_flags |= FILE_FLAG_ERROR;
        return EOF;
    }

    /* if necessary, initialize stream buffer */
    if ( !(stream->m_flags & FILE_FLAG_BUFFER_SET) ) {
        if ( setdefbuf_unlocked(stream) == EOF ) {
            return EOF;
        }
    }

    /* unbuffered files perform direct write */
    if ( stream->m_buffer_mode == _IONBF ) {
        /* if the last access was a read, flush it */
        if ( stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_READ ) {
            if ( fflush_read_unlocked(stream) == EOF ) {
                return EOF;
            }
        }

        /* if stream has no write implementation, return with error */
        if ( !stream->m_impl_write ) {
            errno = EBADF;
            stream->m_flags |= FILE_FLAG_ERROR;
            return EOF;
        }

        /* set stream direction */
        stream->m_flags |= FILE_FLAG_BUFFER_DIRECTION_WRITE;
        stream->m_flags &= ~FILE_FLAG_EOF;

        /* perform writing */
        auto done  = 0;
        auto total = size * count;
        while ( done < total ) {
            /* call write implementation */
            auto buf_ptr       = reinterpret_cast<const u8*>(ptr);
            auto written_bytes = stream->m_impl_write(&(buf_ptr[done]), total, stream);
            if ( !written_bytes ) {
                stream->m_flags |= FILE_FLAG_EOF;
                return EOF;
            } else if ( written_bytes < 0 ) {
                stream->m_flags |= FILE_FLAG_ERROR;
                return EOF;
            }

            done += written_bytes;
        }
        return done / size;
    }

    /* for buffered streams, put char-by-char */
    auto buffer = reinterpret_cast<const u8*>(ptr);
    for ( auto e = 0; e < count; e++ ) {
        auto off = e * size;
        for ( auto b = 0; b < size; b++ ) {
            if ( fputc_unlocked(buffer[off + b], stream) == EOF ) {
                return e;
            }
        }
    }
    return count;
}
