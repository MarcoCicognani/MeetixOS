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
#include <fcntl.h>
#include <stdarg.h>

int open(const char* path, int flags, ...) {
    mode_t creat_mode = 0;

    /* read the creat-mode from the third parameter */
    if ( flags & O_CREAT ) {
        va_list ap;
        va_start(ap, flags);
        creat_mode = va_arg(ap, mode_t);
        va_end(ap);
    }

    /* open the file requested */
    FsOpenStatus open_status;
    FileHandle   fd = s_open_fms(path, flags, creat_mode, &open_status);

    if ( open_status == FS_OPEN_SUCCESSFUL ) {
        return fd;
    } else if ( open_status == FS_OPEN_NOT_FOUND ) {
        errno = ENOENT;
    } else {
        errno = EIO;
    }

    return -1;
}
