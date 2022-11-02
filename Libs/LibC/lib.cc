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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"

#include "lib.hh"
#include "CCliArguments.hh"
#include "stdio.hh"

#include <LibC/signal.h>
#include <LibC/stdio.h>
#include <LibC/stdlib.h>

extern "C" {

void _init();
void _fini();

} /* extern "C" */

extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();
extern "C" void (*__preinit_array_start[])();
extern "C" void (*__preinit_array_end[])();
extern "C" void (*__fini_array_start[])();
extern "C" void (*__fini_array_end[])();

auto libc_main() -> void {
    /* initialize the C runtime environment */
    libc_init();

    /* parse arguments and call application main */
    int ret = EXIT_FAILURE;
    if ( auto parse_error_or_ccli_args = CCliArguments::parse(); parse_error_or_ccli_args.is_value() ) {
        auto const ccli_args = parse_error_or_ccli_args.unwrap();

        ret = main(ccli_args.argc(), ccli_args.argv());
    } else {
        auto const parse_error = parse_error_or_ccli_args.unwrap_error();

        fprintf(stderr, "LibC - Failed to parse cli arguments: %d\n", parse_error.code());
    }

    /* execution finished */
    exit(ret);
}

auto libc_init() -> void {
    /* call pre-init constructors */
    auto const pre_init_array_len = __preinit_array_end - __preinit_array_start;
    for ( auto i = 0; i < pre_init_array_len; i++ )
        (*__preinit_array_start[i])();

    /* call init section */
    _init();

    /* call init constructors */
    auto const init_array_len = __init_array_end - __init_array_start;
    for ( auto i = 0; i < init_array_len; i++ )
        (*__init_array_start[i])();

    /* set default signal handler */
    signal(SIGINT, SIG_DFL);

    /* initialize standard I/O */
    stdio_init();
}

auto libc_fini() -> void {
    /* call all the destructors */
    auto const fini_array_len = __fini_array_end - __fini_array_start;
    for ( auto i = 0; i < fini_array_len; i++ )
        (*__fini_array_start[i])();

    /* call fini section */
    _fini();

    /* finalize the standard I/O */
    stdio_fini();
}

#pragma clang diagnostic pop /* bugprone-reserved-identifier */
