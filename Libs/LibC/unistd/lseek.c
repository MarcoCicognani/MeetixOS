/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *  Ghost, a micro-kernel based operating system for the x86 architecture    *
 *  Copyright (C) 2015, Max Schlüssel <lokoxe@gmail.com>                     *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "errno.h"
#include "Api/Kernel.h"
#include "unistd.h"

/**
 * POSIX wrapper for <s_seek>
 */
off_t lseek(int fd, off_t offset, int whence) {
    // set the seek mode for eva kernel
    FsSeekMode mode;
    if ( whence == SEEK_SET )
        mode = FS_SEEK_SET;
    else if ( whence == SEEK_CUR )
        mode = FS_SEEK_CUR;
    else if ( whence == SEEK_END )
        mode = FS_SEEK_END;

    // performs the syscall
    FsSeekStatus status;
    int64_t      result = s_seek_s(fd, offset, mode, &status);

    // check status
    if ( status == FS_SEEK_SUCCESSFUL )
        return result;
    else if ( status == FS_SEEK_INVALID_FD )
        errno = EBADF;
    else
        errno = EIO;

    return -1;
}
