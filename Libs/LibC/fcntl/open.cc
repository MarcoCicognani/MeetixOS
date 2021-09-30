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
#include <cerrno>
#include <cstdarg>
#include <fcntl.h>

extern "C" int open(const char* path, int flags, ...) {
    /* read the creat-mode from the third parameter */
    auto creat_mode = 0;
    if ( flags & O_CREAT ) {
        va_list ap;
        va_start(ap, flags);
        creat_mode = va_arg(ap, mode_t);
        va_end(ap);
    }

    /* open the file requested */
    FsOpenStatus open_status;
    auto         fd = s_open_fms(path, flags, creat_mode, &open_status);
    if ( open_status == FS_OPEN_SUCCESSFUL )
        return fd;
    else if ( open_status == FS_OPEN_NOT_FOUND )
        errno = ENOENT;
    else
        errno = EIO;
    return -1;
}
