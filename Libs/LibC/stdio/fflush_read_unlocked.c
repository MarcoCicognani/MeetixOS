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

int fflush_read_unlocked(FILE* stream) {
    if ( !(stream->m_flags & FILE_FLAG_BUFFER_DIRECTION_READ) ) {
        return 0;
    }

    /* prohibit reading from a non-read stream */
    if ( !(stream->m_flags & FILE_FLAG_MODE_READ) ) {
        errno = EBADF;
        stream->m_flags |= FILE_FLAG_ERROR;
        return EOF;
    }

    /* get current offset location */
    int   prev_errno       = errno;
    off_t current_position = -1;
    if ( stream->m_impl_seek )
        current_position = stream->m_impl_seek(stream, 0, SEEK_CUR);

    errno = prev_errno;

    /* restore the position */
    int res = 0;
    if ( current_position >= 0 ) {
        size_t buffered_bytes
            = stream->m_buffered_bytes_read - stream->m_buffered_bytes_read_offset;
        off_t restored_position = (uintmax_t)current_position < (uintmax_t)buffered_bytes
                                    ? 0
                                    : current_position - buffered_bytes;

        if ( stream->m_impl_seek(stream, restored_position, SEEK_SET) < 0 )
            stream->m_flags |= FILE_FLAG_ERROR;
    }

    /* reset read buffer offsets */
    stream->m_buffered_bytes_read        = 0;
    stream->m_buffered_bytes_read_offset = 0;

    /* reset stream direction */
    stream->m_flags &= ~FILE_FLAG_BUFFER_DIRECTION_READ;
    return res;
}
