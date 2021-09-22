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
#include "inttypes.h"
#include "stdio.h"
#include "stdio_internal.h"
#include "stdlib.h"

uint8_t  tmpnam_lock   = 0;
char*    tmpnam_static = NULL;
uint64_t tmpnam_next   = 0;

/**
 *
 */
char* tmpnam(char* buf) {
    // lock tmpnam
    s_atomic_lock(&tmpnam_lock);

    // set buffers
    if ( buf == NULL ) {
        // allocate the internal buffer if necessary
        if ( tmpnam_static == NULL ) {
            tmpnam_static = (char*)malloc(L_tmpnam);

            if ( tmpnam_static == NULL ) {
                errno       = ENOMEM;
                tmpnam_lock = 0;
                return NULL;
            }
        }

        // use internal buffer
        buf = tmpnam_static;
    }

    // create temporary name
    ++tmpnam_next;
    pid_t pid;
    snprintf(buf, L_tmpnam, "/sys/temp/%" PRIu64 "-%" PRIu64, tmpnam_next, pid);

    // unlock tmpnam
    tmpnam_lock = 0;

    return buf;
}
