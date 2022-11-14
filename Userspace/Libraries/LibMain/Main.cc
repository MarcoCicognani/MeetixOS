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

#include <LibC/stdlib.h>
#include <LibC/string.h>
#include <LibFmtIO/Err.hh>
#include <LibMain/Main.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Must.hh>

auto runtime_error(Error error) -> int {
    must$(FmtIO::errln("{}Runtime Error{} in {}"sv, FmtIO::foreground(FmtIO::Color::Red), FmtIO::reset(), error));
    return EXIT_FAILURE;
}

auto main(int argc, char const* const* argv) -> int {
    auto error_or_args_vector = Vector<StringView>::try_construct_with_capacity(argc);
    if ( error_or_args_vector.is_error() )
        return runtime_error(error_or_args_vector.unwrap_error());

    /* append all the C-Style arguments into the vector of string views */
    auto args_vector = error_or_args_vector.unwrap();
    for ( auto const i : Range{ 0, argc } )
        args_vector.append_unchecked(StringView::construct_from_cstr(argv[i]));

    /* call the application entry point */
    auto error_or_void = entry(Cxx::move(args_vector));
    if ( error_or_void.is_error() )
        return runtime_error(error_or_void.unwrap_error());
    else
        return EXIT_SUCCESS;
}