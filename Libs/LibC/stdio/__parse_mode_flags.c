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
#include "stdio.h"
#include "stdio_internal.h"
#include "string.h"

/**
 *
 */
int __parse_mode_flags(const char* mode) {
    int flags = 0;

    switch ( *mode ) {
        case 'r':
            flags = FILE_FLAG_MODE_READ;
            break;
        case 'w':
            flags = FILE_FLAG_MODE_WRITE | FILE_FLAG_MODE_CREATE | FILE_FLAG_MODE_TRUNCATE;
            break;
        case 'a':
            flags = FILE_FLAG_MODE_WRITE | FILE_FLAG_MODE_CREATE | FILE_FLAG_MODE_APPEND;
            break;
        default:
            errno = EINVAL;
            return EOF;
    }

    while ( *(++mode) ) {
        switch ( *mode ) {
            case '+':
                flags |= FILE_FLAG_MODE_READ | FILE_FLAG_MODE_WRITE;
                break;
            case 't':
                flags &= ~FILE_FLAG_MODE_BINARY;
                flags |= FILE_FLAG_MODE_TEXTUAL;
                break;
            case 'b':
                flags &= ~FILE_FLAG_MODE_TEXTUAL;
                flags |= FILE_FLAG_MODE_BINARY;
                break;
            default:
                errno = EINVAL;
                return EOF;
        }
    }

    return flags;
}
