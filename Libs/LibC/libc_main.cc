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

#include "libc_main_internal.hh"
#include "stdio/stdio_internal.hh"

#include <Api.h>
#include <clocale>
#include <csignal>
#include <cstdlib>

extern "C" void _init();
extern "C" void _fini();

extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();
extern "C" void (*__preinit_array_start[])();
extern "C" void (*__preinit_array_end[])();
extern "C" void (*__fini_array_start[])();
extern "C" void (*__fini_array_end[])();

void libc_main() {
    /* initialize the C runtime environment */
    libc_init();

    /* parse arguments and call application main */
    int    argc;
    char** argv;
    int    ret = EXIT_FAILURE;
    if ( parse_cli_args(&argc, &argv) ) {
        ret = main(argc, argv);
    } else {
        s_log("failed to parse command line arguments");
    }

    /* execution finished */
    exit(ret);
}

void libc_init() {
    /* call pre-init constructors */
    auto pre_init_array_len = __preinit_array_end - __preinit_array_start;
    for ( auto i = 0; i < pre_init_array_len; i++ ) {
        (*__preinit_array_start[i])();
    }

    /* call init section */
    _init();

    /* call init constructors */
    auto init_array_len = __init_array_end - __init_array_start;
    for ( auto i = 0; i < init_array_len; i++ ) {
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
    auto fini_array_len = __fini_array_end - __fini_array_start;
    for ( auto i = 0; i < fini_array_len; i++ ) {
        (*__fini_array_start[i])();
    }

    /* call fini section */
    _fini();

    /* finalize the standard I/O */
    stdio_fini();
}
