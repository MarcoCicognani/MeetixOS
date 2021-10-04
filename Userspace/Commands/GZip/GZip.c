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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>

/*
 * show help
 */
void show_usages(const char* cmdname) {
    println("");
    println("%s command utility", cmdname);
    println("usage: %s <file.gz> [estract the gz file]", cmdname);
    println("\t%s -h/--help/-?\tshow this help", cmdname);
    println("");
}

/*
 * gz command utility
 */
int main(int argc, char** argv) {
    // create flag for help
    bool showHelp = false;

    // check for help
    if ( getopt_is_help(argc, argv) )
        showHelp = true;

    // if we haven't to show the help
    if ( !showHelp ) {
        // check args
        if ( argc == 2 ) {
            // create path pointer
            char* path = argv[1];

            // get sure for zipped file from extension
            if ( strstr(path, ".gz") != (path + strlen(path) - 3) ) {
                fprintf(stderr,
                        "%s: Not sure if this file is gzipped. Try renaming it to include `.gz' at "
                        "the end\n",
                        argv[0]);
                return 1;
            }

            // open the file
            gzFile source;
            if ( !(source = gzopen(path, "r")) ) {
                fprintf(stderr, "Unable to open the gz file\n");
                return 1;
            }

            // create destination
            char* destname = strdup(path);
            char* t        = strstr(destname, ".gz");
            *t             = '\0';

            // open the new file
            FILE* fdest;
            if ( !(fdest = fopen(destname, "w")) ) {
                fprintf(stderr, "Unable to open the destination file %s\n", destname);
                return 1;
            }

            // read from file
            while ( !gzeof(source) ) {
                char buffer[1024];
                int  read = gzread(source, buffer, 1024);
                fwrite(buffer, read, 1, fdest);
            }

            // close the destination file
            fclose(fdest);
            free(destname);
        }

        else
            fprintf(stderr, "No gzipped filename provided\n");
    }

    else
        show_usages(argv[0]);

    return 0;
}
