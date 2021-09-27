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

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int   g_optind = 1;
char* g_optarg = NULL;
int   g_opterr = 1;
int   g_optopt = 0;

static int has_arg(char c, const char* opt_str) {
    for ( usize i = 0; opt_str[i]; ++i )
        if ( opt_str[i] == c )
            return opt_str[i + 1] == ':' ? required_argument : no_argument;
    return -1;
}

static int has_long_arg(const char** arg, const struct option* long_opts, int* val) {
    for ( usize i = 0; long_opts[i].name; ++i ) {
        usize opt_len = strlen(long_opts[i].name);
        if ( !strncmp(*arg, long_opts[i].name, opt_len) ) {
            *arg += opt_len;
            *val = long_opts[i].val;
            return long_opts[i].has_arg;
        }
    }
    return -1;
}

int getoptishelp(int argc, char** argv) {
    if ( argc <= 1 )
        return false;

    for ( int arg = 0; arg < argc; ++arg ) {
        if ( !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-?") )
            return true;
    }
    return false;
}

usize getopttosize(const char* str) {
    usize val = 0;
    while ( isdigit(*str) )
        val = val * 10 + (*str++ - '0');

    switch ( *str ) {
        case 'K':
        case 'k':
            val *= 1024;
            break;
        case 'M':
        case 'm':
            val *= 1024 * 1024;
            break;
        case 'G':
        case 'g':
            val *= 1024 * 1024 * 1024;
            break;
    }
    return val;
}

int getoptlong(int            argc,
               const char*    argv[],
               const char*    short_opts,
               struct option* long_opts,
               int*           long_index) {
    static usize next_c = 0;
    g_optarg            = NULL;

    if ( long_index )
        g_optind = *long_index;

    while ( g_optind < argc ) {
        const char* str_arg = argv[g_optind];
        if ( !next_c ) {
            /* the first non-option stops the search */
            if ( str_arg[0] != '-' || str_arg[1] == '\0' || (str_arg[1] == '-' && str_arg[2] == '\0') )
                return -1;
            next_c = 1;
        }

        int         val = 0, arg;
        const char* opt_end;
        if ( str_arg[1] == '-' ) {
            if ( !long_opts )
                return -1;

            opt_end = str_arg + 2;
            arg    = has_long_arg(&opt_end, long_opts, &val);
            if ( *opt_end == '=' )
                opt_end++;
        } else {
            /* done with this option element? */
            if ( !str_arg[next_c]  ) {
                ++g_optind;
                next_c = '\0';
                continue;
            }
            val    = str_arg[next_c++];
            arg    = has_arg(val, short_opts);
            opt_end = str_arg + next_c;
        }

        /* unknown option? */
        if ( arg <0 ) {
            /* short option? */
            if ( str_arg[1] != '-' ) {
                if ( g_opterr )
                    fprintf(stderr,
                            "Unrecognized option '%c' in argv[%d]=%s\n",
                            val,
                            g_optind,
                            str_arg);
                g_optopt = val;
            } else if ( g_opterr )
                fprintf(stderr, "Unrecognized argument argv[%d]=%s\n", g_optind, str_arg);
            val = '?';
        }

        /* option with arg? */
        if ( arg == required_argument ) {
            /* the argument is the text following the option or the next cmdline argument */
            if ( *opt_end != '\0' ) {
                g_optarg = (char*)opt_end;
                g_optind++;
                next_c = 0;
            } else if ( g_optind + 1 >= argc ) {
                if ( g_opterr )
                    fprintf(stderr, "Missing argument to option argv[%d]=%s\n", g_optind, str_arg);
                return '?';
            } else {
                g_optarg = (char*)argv[g_optind + 1];
                g_optind += 2;
                next_c = 0;
            }
        } else if ( str_arg[1] == '-' || *opt_end == '\0' ) {
            next_c = '\0';
            g_optind++;
        }
        return val;
    }
    return -1;
}

int getopt(int argc, const char* argv[], const char* short_opts) {
    return getoptlong(argc, argv, short_opts, NULL, NULL);
}
