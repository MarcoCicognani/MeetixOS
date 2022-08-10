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

#include <LibC/stdlib.h>
#include <LibC/string.h>
#include <LibFmtIO/Err.hh>
#include <LibMain/Main.hh>
#include <LibTC/Cxx.hh>

auto main(int argc, char const* const* argv) -> int {
    auto error_or_args_vector = Vector<StringView>::try_construct_with_capacity(argc);
    if ( error_or_args_vector.is_error() ) {
        FmtIO::errln("Failed to construct the arguments Vector: {}"sv, error_or_args_vector.unwrap_error());
        return EXIT_FAILURE;
    }

    /* append all the C-Style arguments into the vector of string views */
    auto args = error_or_args_vector.unwrap();
    for ( auto const i : Range{ 0, argc } )
        args.append_unchecked(StringView{ argv[i], strlen(argv[i]) });

    /* call the application entry point */
    auto error_or_void = entry(Cxx::move(args));
    if ( error_or_void.is_error() ) {
        FmtIO::errln("\033[31;1mRuntime error\033[0m: {}"sv, error_or_void.unwrap_error());
        return EXIT_FAILURE;
    } else
        return EXIT_SUCCESS;
}