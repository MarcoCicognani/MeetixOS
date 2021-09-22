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
#include "ctype.h"
#include "stdint.h"
#include "string.h"
#include "strings.h"

/**
 *
 */
int strcasecmp(const char* str_a, const char* str_b) {
    __DEBUG_TRACE(strcmp);

    uint8_t* mem_a8 = (uint8_t*)str_a;
    uint8_t* mem_b8 = (uint8_t*)str_b;

    for ( ;; ++mem_a8, ++mem_b8 ) {
        uint8_t a = tolower(*mem_a8);
        uint8_t b = tolower(*mem_b8);

        if ( a == 0 && b == 0 ) {
            return 0;
        } else if ( a < b ) {
            return -1;
        } else if ( a > b ) {
            return 1;
        }
    }

    return 0;
}
