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

#include <eva.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * show the help
 */
void usage(const char* cmdname) {
    println("");
    println("File Reader command utility");
    println("usage: %s -opt <file>", cmdname);
    println("avaible options:");
    println("\t-s [ascii/utf8 read]");
    println("\t-x [binary read]");
    println("");
}

/*
 * read utils
 */
int main(int argc, const char* argv[]) {
    // mode flags
    bool strmode  = true;
    bool examode  = false;
    bool showHelp = false;

    // parse arguments
    char opt;
    while ( (opt = getopt(argc, argv, "sxh?")) != EOF ) {
        switch ( opt ) {
            case 's':
                strmode = true;
                break;
            case 'x':
                examode = true;
                strmode = false;
                break;
            case 'h':
                showHelp = true;
                break;
            case '?':
                showHelp = true;
                break;
        }
    }

    // if we haven't to show the help
    if ( !showHelp ) {
        // file descriptor opened
        File_t file = FD_NONE;

        // open the file
        int i = 0;
        while ( (i < argc) && (file = OpenF(argv[i++], O_RDONLY)) != -1 )

            // check if file is opened, else exit
            if ( file != -1 ) {
                // allocate buffer with length of file
                uint32_t length = Length(file);
                uint8_t  buffer[length];

                // read the file
                uint32_t readed = 0;
                if ( (readed = Read(file, buffer, length)) < length ) {
                    fprintf(stderr,
                            "Unable to read all the file, readed %d bytes on %d bytes\n",
                            readed,
                            length);
                    return 1;
                }

                // print on screen with the format provided
                if ( strmode )
                    printf("%s", buffer);
                else if ( examode )
                    printf("%x", buffer);

                // close
                Close(file);
            }

            else {
                fprintf(stderr, "No valid filename provided\n");
                return 1;
            }
    }

    else
        usage(argv[0]);
}
