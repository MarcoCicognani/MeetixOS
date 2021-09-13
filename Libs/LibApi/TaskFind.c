/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "eva/user.h"

#include <malloc.h>

/**
 *
 */
uint32_t TaskFind(const char* name, int maxTries) {
    uint32_t id    = -1;
    int      tries = 0;

    while ( id == -1 && tries < maxTries ) {
        id = TaskGetID(name);
        ++tries;

        if ( id == -1 ) {
            // TODO limit tries
            Sleep(100);
        }
    }

    if ( id == -1 ) {
        // TODO set errno
    }

    return id;
}
