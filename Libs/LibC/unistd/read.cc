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

#include <Api.h>
#include <errno.h>
#include <unistd.h>

extern "C" isize read(int fd, void* buf, usize count) {
    FsReadStatus read_status;
    usize        read_bytes = s_read_s(fd, buf, count, &read_status);
    if ( read_status == FS_READ_SUCCESSFUL )
        return (isize)read_bytes;
    else if ( read_status == FS_READ_INVALID_FD )
        errno = EBADF;
    else if ( read_status == FS_READ_BUSY )
        errno = EIO;
    else
        errno = EINVAL;
    return -1;
}
