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

extern "C" off_t lseek(int fd, off_t offset, int whence) {
    FsSeekMode mode;
    if ( whence == SEEK_SET )
        mode = FS_SEEK_SET;
    else if ( whence == SEEK_CUR )
        mode = FS_SEEK_CUR;
    else if ( whence == SEEK_END )
        mode = FS_SEEK_END;

    /* perform the seek operation */
    FsSeekStatus seek_status;
    i64          new_offset = s_seek_s(fd, offset, mode, &seek_status);
    if ( seek_status == FS_SEEK_SUCCESSFUL )
        return new_offset;
    else if ( seek_status == FS_SEEK_INVALID_FD )
        errno = EBADF;
    else
        errno = EIO;
    return -1;
}
