/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <Api.h>
#include <LibC/ctype.h>
#include <LibC/libgen.h>
#include <LibC/stdlib.h>
#include <LibC/string.h>

#define SKIP_WHITESPACE(pos)                                                                                                               \
    do {                                                                                                                                   \
        while ( *pos && isspace(*pos) )                                                                                                    \
            ++pos;                                                                                                                         \
    } while ( 0 )

#define SKIP_ARGUMENT(pos)                                                                                                                 \
    do {                                                                                                                                   \
        auto instr = false;                                                                                                                \
        auto esc   = false;                                                                                                                \
        while ( *pos ) {                                                                                                                   \
            if ( *pos == '"' && !esc ) {                                                                                                   \
                instr = !instr;                                                                                                            \
                esc   = false;                                                                                                             \
            } else if ( *pos == '\\' && !esc )                                                                                             \
                esc = true;                                                                                                                \
            else if ( isspace(*pos) && !instr && !esc )                                                                                    \
                break;                                                                                                                     \
            else                                                                                                                           \
                esc = false;                                                                                                               \
                                                                                                                                           \
            ++pos;                                                                                                                         \
        }                                                                                                                                  \
    } while ( 0 )

static char* get_executable_name() {
    /* obtain the executable path of this process */
    auto exe_path_buffer = new char[PATH_MAX];
    s_get_executable_path(exe_path_buffer);

    /* find the basename of the  */
    auto exe_base_name = basename(exe_path_buffer);

    /* copy base name into smaller buffer */
    char* exe_name = nullptr;
    auto  len      = strlen(exe_base_name);
    if ( len < PATH_MAX ) {
        exe_name = new char[len + 1];
        strcpy(exe_name, exe_base_name);
    }

    /* free the absolute path */
    free(exe_path_buffer);
    return exe_name;
}

extern "C" bool parse_cli_args(int* out_argc, char*** out_args) {
    /* obtain raw command line string from the kernel */
    auto raw_cli_args = new char[CLIARGS_BUFFER_LENGTH];
    s_cli_args_release(raw_cli_args);

    /* prepare arguments count */
    auto cli_ptr = raw_cli_args;
    auto argc    = 1;
    while ( *cli_ptr ) {
        SKIP_WHITESPACE(cli_ptr);
        SKIP_ARGUMENT(cli_ptr);
        ++argc;
    }

    /* allocate the buffer than put the executable name as first argument */
    auto argv = new char*[argc + 1];
    argv[0]   = get_executable_name();

    /* put the executable name as first argument */
    auto arg_pos = 1;
    cli_ptr      = raw_cli_args;
    while ( *cli_ptr ) {
        SKIP_WHITESPACE(cli_ptr);
        argv[arg_pos++] = cli_ptr;
        SKIP_ARGUMENT(cli_ptr);
        if ( !*cli_ptr )
            break;

        /* separate the arguments */
        *cli_ptr++ = '\0';
    }
    argv[arg_pos] = nullptr;

    /* cleanup the arguments */
    for ( auto i = 1; i < argc; i++ ) {
        auto arg     = argv[i];
        auto arg_len = strlen(arg);

        auto esc = false;
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
    return true;
}
