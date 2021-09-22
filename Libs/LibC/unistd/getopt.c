/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License,  or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful, 				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not,  write to the Free Software                    *
 * Foundation,  Inc.,  51 Franklin Street,  Fifth Floor,  Boston,  MA  02110-1301  USA *
 **********************************************************************************/

#include <Api.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int   optind = 1;
char* optarg = NULL;
int   opterr = 1;
int   optopt = 0;

/*
 *
 */
static int hasarg(char c, const char* optstring) {
    for ( size_t i = 0; optstring[i]; ++i )
        if ( optstring[i] == c )
            return optstring[i + 1] == ':' ? required_argument : no_argument;

    return -1;
}

/*
 *
 */
static int haslongarg(const char** arg, const struct option* longopts, int* val) {
    for ( size_t i = 0; longopts[i].name; ++i ) {
        size_t optlen = strlen(longopts[i].name);
        if ( !strncmp(*arg, longopts[i].name, optlen) ) {
            *arg += optlen;
            *val = longopts[i].val;
            return longopts[i].has_arg;
        }
    }
    return -1;
}

/**
 * Checks whether the given arguments may be a kind of help-request. That means one of:
 * <prog> --help
 * <prog> -h
 * <prog> -?
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @return true if it is a help-request
 */
int getoptishelp(int argc, char** argv) {
    // if there are no args return
    if ( argc <= 1 )
        return false;

    // check on all args
    for ( int arg = 0; arg < argc; ++arg )
        if ( !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-?") )
            return true;

    return false;
}

/**
 * Converts the given string to a number, supporting the suffixes K,M, and G (lower and upper case)
 *
 * @param str:		the string
 * @return the number
 */
size_t getopttosize(const char* str) {
    size_t val = 0;
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

/**
 * Parses the given arguments for given options.
 *
 * <optstring> is used for short options ("-o") and consists of the option characters. A ':' after
 * an option character specifies that the option has an argument.
 * If an option is found, the character is returned. If it has an argument, <optarg> points to the
 * argument (either the text after the option character or the text of the next item in argv).
 *
 * <longopts> can optionally be specified for long options ("--option"). The last array element
 * needs to consist of zeros. <name> specifies the option name ("option"), <has_arg> specifies
 * whether it has an argument (*_argument) and <val> the value to return if that option is found.
 *
 * @param argc:			the number of args, as received in main
 * @param argv:			the arguments, as received in main
 * @param optstring:	the short options specification
 * @param longopts:		the long options specification
 * @param longindex:	if non-NULL, optind will be set to *longindex
 * @return the option or -1 if no option has been found
 */
int getoptlong(int            argc,
               const char*    argv[],
               const char*    optstring,
               struct option* longopts,
               int*           longindex) {
    static size_t nextchar = 0;
    optarg                 = NULL;

    if ( longindex )
        optind = *longindex;

    while ( optind < argc ) {
        const char* optel = argv[optind];
        if ( nextchar == 0 ) {
            /* the first non-option stops the search */
            if ( optel[0] != '-' || optel[1] == '\0' || (optel[1] == '-' && optel[2] == '\0') )
                return -1;
            nextchar = 1;
        }

        int         val = 0, arg;
        const char* optend;
        if ( optel[1] == '-' ) {
            if ( !longopts )
                return -1;
            optend = optel + 2;
            arg    = haslongarg(&optend, longopts, &val);
            if ( *optend == '=' )
                optend++;
        } else {
            /* done with this option element? */
            if ( optel[nextchar] == '\0' ) {
                optind++;
                nextchar = 0;
                continue;
            }
            val    = optel[nextchar++];
            arg    = hasarg(val, optstring);
            optend = optel + nextchar;
        }

        /* unknown option? */
        if ( arg == -1 ) {
            /* short option? */
            if ( optel[1] != '-' ) {
                if ( opterr )
                    fprintf(stderr,
                            "Unrecognized option '%c' in argv[%d]=%s\n",
                            val,
                            optind,
                            optel);
                optopt = val;
            } else if ( opterr )
                fprintf(stderr, "Unrecognized argument argv[%d]=%s\n", optind, optel);
            val = '?';
        }

        /* option with arg? */
        if ( arg == required_argument ) {
            /* the argument is the text following the option or the next cmdline argument */
            if ( *optend != '\0' ) {
                optarg = (char*)optend;
                optind++;
                nextchar = 0;
            } else if ( optind + 1 >= argc ) {
                if ( opterr )
                    fprintf(stderr, "Missing argument to option argv[%d]=%s\n", optind, optel);
                return '?';
            } else {
                optarg = (char*)argv[optind + 1];
                optind += 2;
                nextchar = 0;
            }
        } else if ( optel[1] == '-' || *optend == '\0' ) {
            nextchar = 0;
            optind++;
        }
        return val;
    }
    return -1;
}

/**
 * Parses the given arguments for given options.
 *
 * <optstring> is used for short options ("-o") and consists of the option characters. A ':' after
 * an option character specifies that the option has an argument.
 * If an option is found, the character is returned. If it has an argument, <optarg> points to the
 * argument (either the text after the option character or the text of the next item in argv).
 *
 * <longopts> can optionally be specified for long options ("--option"). The last array element
 * needs to consist of zeros. <name> specifies the option name ("option"), <has_arg> specifies
 * whether it has an argument (*_argument) and <val> the value to return if that option is found.
 *
 * @param argc:			the number of args, as received in main
 * @param argv:			the arguments, as received in main
 * @param optstring:	the short options specification
 * @param longopts:		the long options specification
 * @param longindex:	if non-NULL, optind will be set to *longindex
 * @return the option or -1 if no option has been found
 */
int getopt(int argc, const char* argv[], const char* optstring) {
    return getoptlong(argc, argv, optstring, NULL, NULL);
}
