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

int fflush_write_unlocked(FILE* stream) {
    if ( !(stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_WRITE) )
        return 0;

    /* prohibit reading from a non-write stream */
    if ( !(stream->m_flags & FILE_FLAG_MODE_WRITE) ) {
        errno = EBADF;
        stream->m_flags |= FILE_FLAG_ERROR;
        return EOF;
    }

    /* if stream has no write implementation, return with error */
    if ( !stream->m_impl_write ) {
        errno = EBADF;
        stream->m_flags |= FILE_FLAG_ERROR;
        return EOF;
    }

    /* perform writing */
    int   res   = 0;
    usize done  = 0;
    usize total = stream->m_buffered_bytes_write;
    while ( done < total ) {
        /* call write implementation */
        isize written_bytes
            = stream->m_impl_write((void*)(stream->m_io_buffer + done), total - done, stream);

        if ( !written_bytes ) {
            stream->m_flags |= FILE_FLAG_EOF;
            res = EOF;
            break;
        } else if ( written_bytes < 0 ) {
            stream->m_flags |= FILE_FLAG_ERROR;
            res = EOF;
            break;
        }

        done += written_bytes;
    }

    /* cleanup stream write buffer flags */
    stream->m_flags &= ~FILE_FLAG_BUFFER_DIRECTION_WRITE;
    stream->m_buffered_bytes_write = 0;
    return res;
}
