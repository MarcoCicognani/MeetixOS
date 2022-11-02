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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibApi/Api.h>
#include <LibC/errno.h>
#include <LibC/fcntl.h>
#include <stdarg.h>

extern "C" {

int open(const char* path, int flags, ...) {
    /* read the creat-mode from the third parameter */
    int creat_mode = 0;
    if ( flags & O_CREAT ) {
        va_list ap;
        va_start(ap, flags);
        creat_mode = va_arg(ap, mode_t);
        va_end(ap);
    }

    /* open the file requested */
    FsOpenStatus open_status;

    auto fd = s_open_fms(path, flags, creat_mode, &open_status);
    if ( open_status == FS_OPEN_SUCCESSFUL )
        return fd;
    else if ( open_status == FS_OPEN_NOT_FOUND )
        errno = ENOENT;
    else
        errno = EIO;
    return -1;
}

} /* extern "C" */

#pragma clang diagnostic pop
