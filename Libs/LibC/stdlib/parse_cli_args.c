/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <Api.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define SKIP_WHITESPACE(pos)                                                                       \
    while ( *pos && isspace(*pos) ) {                                                              \
        ++pos;                                                                                     \
    }

#define SKIP_ARGUMENT(pos)                                                                         \
    bool instr = false;                                                                            \
    bool esc   = false;                                                                            \
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

static char* get_executable_name() {
    /* obtain the executable path of this process */
    char* exe_path_buffer = (char*)malloc(PATH_MAX);
    s_get_executable_path(exe_path_buffer);

    /* find the basename of the  */
    char* exe_base_name = basename(exe_path_buffer);

    /* copy base name into smaller buffer */
    char*  exe_name = NULL;
    size_t len      = strlen(exe_base_name);
    if ( len < PATH_MAX ) {
        exe_name = (char*)malloc(len + 1);
        strcpy(exe_name, exe_base_name);
    }

    /* free the absolute path */
    free(exe_path_buffer);
    return exe_name;
}

int parse_cli_args(int* out_argc, char*** out_args) {
    /* obtain raw command line string from the kernel */
    char* raw_cli_args = (char*)malloc(CLIARGS_BUFFER_LENGTH);
    if ( !raw_cli_args ) {
        errno = ENOMEM;
        return -1;
    }
    s_cli_args_release(raw_cli_args);

    /* prepare arguments count */
    char* cli_ptr = raw_cli_args;
    int   argc    = 1;
    while ( *cli_ptr ) {
        SKIP_WHITESPACE(cli_ptr);
        SKIP_ARGUMENT(cli_ptr);
        ++argc;
    }

    /* allocate the buffer for the parsed arguments */
    char** argv = (char**)malloc(sizeof(char*) * (argc + 1));
    if ( !argv ) {
        errno = ENOMEM;
        return -1;
    }

    /* put the executable name as first argument */
    argv[0]     = get_executable_name();
    cli_ptr     = raw_cli_args;
    int arg_pos = 1;
    while ( *cli_ptr ) {
        SKIP_WHITESPACE(cli_ptr);
        argv[arg_pos++] = cli_ptr;
        SKIP_ARGUMENT(cli_ptr);
        if ( !*cli_ptr )
            break;

        /* separate the arguments */
        *cli_ptr++ = '\0';
    }
    argv[arg_pos] = NULL;

    /* cleanup the arguments */
    for ( int i = 1; i < argc; i++ ) {
        char*  arg    = argv[i];
        size_t arg_len = strlen(arg);

        bool esc = false;
        for ( int p = 0; p < arg_len; p++ ) {
            if ( !esc && arg[p] == '\\' ) {
                for ( int x = p; x < arg_len; x++ ) {
                    arg[x] = arg[x + 1];
                }
                arg_len--;
                --p; /* repeat on same position */
                esc = true;
            } else if ( !esc && arg[p] == '"' ) {
                for ( int x = p; x < arg_len; x++ ) {
                    arg[x] = arg[x + 1];
                }
                arg_len--;
                --p; /* repeat on same position */
                esc = false;

            } else {
                esc = false;
            }
        }
    }

    /* write out parameters */
    *out_argc = argc;
    *out_args = argv;
    return 0;
}
