/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                  *
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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <eva.h>

/**
 *
 */
int setenv(const char* key, const char* val, int overwrite) {
    // concats the arguments
    char args[1024];
    sprintf(args, "--sentenv %s=%s", key, val);

    // exec shell and check status
    if ( Spawn("/Bins/MxSh", args, "/", SECURITY_LEVEL_APPLICATION) == SPAWN_STATUS_SUCCESSFUL )
        return 0;

    return -1;
}
