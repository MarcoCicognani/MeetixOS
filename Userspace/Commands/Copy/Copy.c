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

#include <Api.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * show the help
 */
void usage(const char* cmdname) {
    println("");
    println("Copy command utility");
    println("usage: %s <path/to/source> <path/to/dest>", cmdname);
    println("aviable filters");
    println("\t-h/--help/-?\tshow thos help");
    println("");
}

/*
 * copy tool command
 */
int main(int argc, char* argv[]) {
    // create flag for help
    bool showHelp = false;

    // search on arguments for help request
    if ( getoptishelp(argc, argv) )
        showHelp = true;

    // only if we haven't to show the help
    if ( !showHelp ) {
        // if are provided all arguments
        if ( argc == 3 ) {
            // copy the arguments
            char* pathsrc  = argv[1];
            char* pathdest = argv[2];

            // open the file
            FILE* tocopy = fopen(pathsrc, "rb");
            if ( tocopy == NULL ) {
                fprintf(stderr, "file %s not found", pathsrc);
                return 1;
            }

            // create the destination file
            FILE* dest = fopen(pathdest, "w");

            // create a buffer with size of file
            uint32_t buffsize = s_length(fileno(tocopy));
            uint8_t  buffer[buffsize];

            // read and write
            ssize_t length;
            while ( (length = fread(buffer, 1, buffsize, tocopy)) > 0 )
                ;

            // write on new file
            if ( fwrite(buffer, 1, buffsize, dest) != buffsize ) {
                fprintf(stderr, "an error was occoured while writing file %s\n", pathdest);
                return 1;
            }
        }

        else {
            fprintf(stderr, "No arguments are provided");
            usage(argv[0]);
            return 1;
        }
    }

    else
        usage(argv[0]);

    return 0;
}
