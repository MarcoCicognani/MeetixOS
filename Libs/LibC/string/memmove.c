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

#include "eva.h"
#include "stdint.h"
#include "string.h"

/**
 *
 */
void* memmove(void* dest, const void* src, size_t num) {
    __DEBUG_TRACE(memmove);

    if ( dest > src ) {
        uint8_t* src_8  = ((uint8_t*)src) + num - 1;
        uint8_t* dest_8 = ((uint8_t*)dest) + num - 1;
        while ( num-- ) {
            *dest_8-- = *src_8--;
        }

    }

    else {
        uint8_t* src_8  = (uint8_t*)src;
        uint8_t* dest_8 = (uint8_t*)dest;
        while ( num-- ) {
            *dest_8++ = *src_8++;
        }
    }

    return dest;
}
