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
 * POSIX wrapper for <s_write>
 */
ssize_t write(int fd, const void* buf, size_t count) {
    // performs write
    FsWriteStatus stat;
    int32_t       len = s_write_s(fd, buf, count, &stat);

    // check return status
    if ( stat == FS_WRITE_SUCCESSFUL )
        return len;
    else if ( stat == FS_WRITE_INVALID_FD )
        errno = EBADF;
    else if ( stat == FS_WRITE_BUSY )
        errno = EIO;
    else
        errno = EIO;

    return -1;
}
