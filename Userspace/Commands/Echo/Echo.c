/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include <stdio.h>
#include <string.h>

/**
 * echo command utility
 */
int main(int argc, char** argv) {
    bool newline = true;
    if ( argc == 1 ) {
        int c;
        while ( (c = getchar()) > 0 )
            printf("%c", c);
    } else {
        for ( int i = 1; i < argc; i++ ) {
            if ( i == 1 && !strcmp(argv[i], "-n") ) {
                newline = false;
                continue;
            }

            if ( i > 1 && !(!newline && i == 2) )
                printf(" ");
            printf("%s", argv[i]);
        }
    }

    if ( newline )
        printf("\n");
}
