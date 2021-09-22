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

#include "Api.h"
#include "libgen.h"
#include "string.h"

static char _statbuf[8];

/**
 *
 */
char* dirname(char* path) {
    // return dot if null
    if ( path == NULL ) {
        _statbuf[0] = '.';
        _statbuf[1] = 0;
        return _statbuf;
    }

    // get length of the path
    size_t len = strlen(path);

    // return dot if empty
    if ( len == 0 ) {
        _statbuf[0] = '.';
        _statbuf[1] = 0;
        return _statbuf;
    }

    // overwrite trailing slashes with nulls
    char* last = path + len - 1;
    while ( last >= path && *last == '/' ) {
        *last = 0;
        --last;
    }

    // if the entire path consisted of slashes, return slash
    if ( *path == 0 ) {
        _statbuf[0] = '/';
        _statbuf[1] = 0;
        return _statbuf;
    }

    // check for slash
    char* rightmostSlash = strrchr(path, '/');

    if ( rightmostSlash == NULL ) {
        _statbuf[0] = '.';
        _statbuf[1] = 0;
        return _statbuf;
    }

    // null-terminate at end
    if ( rightmostSlash == path ) {
        rightmostSlash[1] = 0;
    } else {
        rightmostSlash[0] = 0;
    }

    return path;
}
