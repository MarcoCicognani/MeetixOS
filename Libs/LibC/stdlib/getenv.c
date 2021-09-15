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
char* getenv(const char* key) {
    // open the environment file
    FILE* env = fopen("/MeetiX/Configs/Environ/Global", "r");

    // create the line buffer
    const int buflen = 1024;
    const int keylen = strlen(key);
    char*     line   = malloc(sizeof(char) * buflen);

    // read the file
    while ( readline(env, line) ) {
        // get separator character
        char* separator = strchr(line, '=');

        // compare the provided key
        if ( keylen == (int)(separator - line) && !strncmp(key, line, keylen) ) {
            // clos the file
            fclose(env);

            // return the value
            return ++separator;
        }

        // reset the buffer
        memset(line, '\0', buflen);
    }

    // variable not found, close file and return
    fclose(env);
    return NULL;
}
