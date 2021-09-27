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

#include "main_internal.h"
#include "stdio/stdio_internal.h"

#include <Api.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>

extern void _init();
extern void _fini();

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();
extern void (*__preinit_array_start[])();
extern void (*__preinit_array_end[])();
extern void (*__fini_array_start[])();
extern void (*__fini_array_end[])();

void libc_main() {
    int ret = EXIT_FAILURE;

    /* parse arguments and call application main */
    int    argc;
    char** args;
    if ( !parse_cli_args(&argc, &args) ) {
        ret = main(argc, args);
    } else {
        s_log("failed to parse command line arguments");
    }

    /* execution finished */
    exit(ret);
}

void libc_init() {
    /* call all the constructors */
    for ( size_t i = 0; i < __preinit_array_end - __preinit_array_start; i++ ) {
        (*__preinit_array_start[i])();
    }

    _init();

    for ( size_t i = 0; i < __init_array_end - __init_array_start; i++ ) {
        (*__init_array_start[i])();
    }

    /* set default locale */
    setlocale(LC_ALL, "C");

    /* set default signal handler */
    signal(SIGINT, SIG_DFL);

    /* initialize standard I/O */
    stdio_init();
}

/**
 * Finalize the C library
 */
void libc_fini() {
    /* call all the destructors */
    for ( size_t i = 0; i < __fini_array_end - __fini_array_start; i++ ) {
        (*__fini_array_start[i])();
    }

    _fini();

    /* finalize the standard I/O */
    stdio_fini();
}
