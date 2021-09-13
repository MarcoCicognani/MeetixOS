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
 * Read text on <stream> until \n and store it to buffer
 * if a NULL stream is provided function return EOF, if buffer is provided NULL
 * the function allocate a buffer with malloc
 *
 * @param stream:       descriptor of file to read
 * @param buffer:       the buffer where store readed characters
 * @return 0 if the eof is reached
 */
uint8_t readline(FILE* stream, char* buffer) {
    // stream and buffer check
    if ( !stream )
        return 0;
    if ( !buffer )
        buffer = malloc(sizeof(char) * 5000);

    // read all the line
    char c;
    int  count = 0;
    while ( ((c = fgetc(stream)) != '\n') && (c != EOF) )
        buffer[count++] = c;

    // close the buffer
    buffer[count] = '\0';
    return (c != EOF);
}