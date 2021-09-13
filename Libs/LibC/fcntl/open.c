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
#include "eva.h"
#include "fcntl.h"

#include <stdarg.h>

/**
 *
 */
int open(const char* pathname, int flags, ...) {
    mode_t       mode = 0;
    FsOpenStatus status;

    // if the create flag is given, we must have a mode
    if ( flags & O_CREAT ) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, mode_t);
        va_end(ap);
    }

    // perform opening syscall
    File_t fd = OpenFS(pathname, flags, &status);

    if ( status == FS_OPEN_SUCCESSFUL )
        return fd;
    else if ( status == FS_OPEN_NOT_FOUND )
        errno = ENOENT;
    else
        errno = EIO;

    return -1;
}
