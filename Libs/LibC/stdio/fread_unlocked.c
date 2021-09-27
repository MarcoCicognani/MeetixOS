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
#include <stdio.h>

usize fread_unlocked(const void* ptr, usize size, usize count, FILE* stream) {
    if ( !size || !count )
        return 0;

    /* check if stream is writable */
    if ( !(stream->m_flags & FILE_FLAG_MODE_READ) ) {
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

    /* unbuffered files perform direct read */
    if ( stream->m_buffer_mode == _IONBF ) {
        /* if the last access was write, flush it */
        if ( stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_WRITE ) {
            if ( fflush_write_unlocked(stream) == EOF ) {
                return EOF;
            }
        }

        /* if stream has no read implementation, return with error */
        if ( stream->m_impl_read == NULL ) {
            errno = EBADF;
            stream->m_flags |= FILE_FLAG_ERROR;
            return EOF;
        }

        /* set stream flags */
        stream->m_flags |= FILE_FLAG_BUFFER_DIRECTION_READ;
        stream->m_flags &= ~FILE_FLAG_EOF;

        // perform reading
        usize done  = 0;
        usize total = size * count;
        while ( done < total ) {
            /* call read implementation */
            isize read_bytes = stream->m_impl_read(&(((u8*)ptr)[done]), total, stream);
            if ( !read_bytes ) {
                stream->m_flags |= FILE_FLAG_EOF;
                break;
            } else if ( read_bytes < 0 ) {
                stream->m_flags |= FILE_FLAG_ERROR;
                break;
            }

            done += read_bytes;
        }
        return done / size;
    }

    /* for buffered streams, get char-by-char */
    u8* buffer = (u8*)ptr;
    for ( usize e = 0; e < count; e++ ) {
        usize off = e * size;
        for ( usize b = 0; b < size; b++ ) {
            int c = fgetc_unlocked(stream);
            if ( c == EOF ) {
                return e;
            }
            buffer[off + b] = (u8)c;
        }
    }
    return count;
}
