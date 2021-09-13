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

#include "ctype.h"
#include "eva.h"
#include "libgen.h"
#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/**
 *
 */
#define SKIP_WHITESPACE(pos)                                                                       \
    while ( *pos && isspace(*pos) ) {                                                              \
        ++pos;                                                                                     \
    }

/**
 *
 */
#define SKIP_ARGUMENT(pos)                                                                         \
    uint8_t instr = false;                                                                         \
    uint8_t esc   = false;                                                                         \
    while ( *pos ) {                                                                               \
        if ( *pos == '"' && !esc ) {                                                               \
            instr = !instr;                                                                        \
            esc   = false;                                                                         \
        } else if ( *pos == '\\' && !esc ) {                                                       \
            esc = true;                                                                            \
        } else if ( isspace(*pos) && !instr && !esc ) {                                            \
            break;                                                                                 \
        } else {                                                                                   \
            esc = false;                                                                           \
        }                                                                                          \
        ++pos;                                                                                     \
    }

/**
 *
 */
char* get_executable_name() {
    // read path for the executable
    char* absoluteExecPath = (char*)malloc(PATH_MAX);
    GetExecutablePath(absoluteExecPath);

    // find base name
    char* execBaseName = basename(absoluteExecPath);

    // copy base name into smaller buffer
    char* execName = 0;

    size_t len = strlen(execBaseName);
    if ( len < PATH_MAX ) {
        execName = (char*)malloc(len + 1);
        strcpy(execName, execBaseName);
    }

    // free the absolute buffer
    free(absoluteExecPath);

    return execName;
}

/**
 * Prepares command line arguments
 */
int parseargs(int* out_argc, char*** out_args) {
    // get argument line from system
    char* unparsedCommandLine = (char*)malloc(CLIARGS_BUFFER_LENGTH);
    if ( unparsedCommandLine == NULL ) {
        return -1;
    }
    CliArgsRelease(unparsedCommandLine);

    // count arguments
    char* pos  = unparsedCommandLine;
    int   argc = 1;
    while ( *pos ) {
        SKIP_WHITESPACE(pos);
        SKIP_ARGUMENT(pos);
        ++argc;
    }

    // create argument array
    char** argv = (char**)malloc(sizeof(char*) * (argc + 1));
    if ( argv == NULL ) {
        return -1;
    }

    // put executable path as first argument
    argv[0] = get_executable_name();

    pos       = unparsedCommandLine;
    int argpp = 1;
    while ( *pos ) {
        SKIP_WHITESPACE(pos);
        argv[argpp++] = pos;
        SKIP_ARGUMENT(pos);
        if ( *pos == 0 ) {
            break;
        }
        *pos++ = 0;
    }

    argv[argpp] = 0;

    // clean up arguments
    for ( int i = 1; i < argc; i++ ) {
        char*  arg    = argv[i];
        size_t arglen = strlen(arg);

        uint8_t esc = false;
        for ( int p = 0; p < arglen; p++ ) {
            if ( !esc && arg[p] == '\\' ) {
                for ( int x = p; x < arglen; x++ ) {
                    arg[x] = arg[x + 1];
                }
                arglen--;
                --p; // repeat on same position
                esc = true;

            } else if ( !esc && arg[p] == '"' ) {
                for ( int x = p; x < arglen; x++ ) {
                    arg[x] = arg[x + 1];
                }
                arglen--;
                --p; // repeat on same position
                esc = false;

            } else {
                esc = false;
            }
        }
    }

    // write out parameters
    *out_argc = argc;
    *out_args = argv;

    return 0;
}
