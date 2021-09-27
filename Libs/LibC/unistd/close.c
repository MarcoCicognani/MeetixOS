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

int close(int fd) {
    FsCloseStatus status = s_close(fd);
    if ( status == FS_CLOSE_SUCCESSFUL )
        return 0;
    else if ( status == FS_CLOSE_INVALID_FD )
        errno = EBADF;
    else
        errno = EIO;
    return -1;
}
