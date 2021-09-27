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

isize write(int fd, const void* buf, usize count) {
    FsWriteStatus write_status;
    usize         written_bytes = s_write_s(fd, buf, count, &write_status);
    if ( write_status == FS_WRITE_SUCCESSFUL )
        return (isize)written_bytes;
    else if ( write_status == FS_WRITE_INVALID_FD )
        errno = EBADF;
    else if ( write_status == FS_WRITE_BUSY )
        errno = EIO;
    else
        errno = EINVAL;
    return -1;
}
