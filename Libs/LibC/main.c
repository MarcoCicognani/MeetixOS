/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 * 																			     *
 * This program is free software; you can redistribute it and/or * modify it
 *under the terms of the GNU General Public License                    * as
 *published by the Free Software Foundation; either version 2
 ** of the License, or (char *argumentat your option) any later version.
 **
 *																				 *
 * This program is distributed in the hope that it will be useful,
 ** but WITHout ANY WARRANTY; without even the implied warranty of *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software * Foundation,
 *Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "eva.h"
#include "locale.h"
#include "main_internal.h"
#include "signal.h"
#include "stdio/stdio_internal.h"
#include "stdlib.h"

#include <stdio.h>

/**
 * Global constructor routine
 */
extern void _init();
extern void _fini();

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();
extern void (*__preinit_array_start[])();
extern void (*__preinit_array_end[])();
extern void (*__fini_array_start[])();
extern void (*__fini_array_end[])();

/**
 * Application entry routine, called by the CRTs.
 */
int __main() {
    // initialize libc
    _InitLibc();

    // call global constructors
    for ( size_t i = 0; i < __preinit_array_end - __preinit_array_start; i++ ) {
        (*__preinit_array_start[i])();
    }

    _init();

    for ( size_t i = 0; i < __init_array_end - __init_array_start; i++ ) {
        (*__init_array_start[i])();
    }

    // default return value
    int ret = EXIT_FAILURE;

    // parse arguments and call application main
    int    argc;
    char** args;

    // parse args and call application main
    if ( !parseargs(&argc, &args) ) {
        ret = main(argc, args);
    } else {
        Log("failed to parse command line arguments");
    }

    // leave
    exit(ret);
}

/**
 * Initializes the C library
 */
void _InitLibc() {
    // set default locale (N1548-7.11.1.1-4)
    setlocale(LC_ALL, "C");

    // set default signal handlers
    signal(SIGINT, SIG_DFL);

    // initialize standard I/O
    __init_stdio();
}

/**
 * Finalize the C library
 */
void _FiniLibc() {
    // call global destructors
    for ( size_t i = 0; i < __fini_array_end - __fini_array_start; i++ ) {
        (*__fini_array_start[i])();
    }

    _fini();

    // Finalize the standard I/O
    __fini_stdio();
}
