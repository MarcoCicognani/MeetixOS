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

#pragma once

#include <LibC/stdio.h>
#include <LibTC/Collection/StringBuilder.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Text/Format.hh>

namespace FmtIO {

enum class Color : i32 {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    LGreen
};

auto foreground(Color foreground) -> StringView;
auto background(Color background) -> StringView;
auto reset() -> StringView;

template<typename... TArgs>
auto vout(FILE* file, StringView format_view, TArgs&&... args) -> ErrorOr<void> {
    /* format the arguments according to the given <format_view> */
    auto string_builder = StringBuilder::construct_empty();
    TRY(format(string_builder, format_view, Cxx::forward<TArgs>(args)...));

    /* write the result into the stdout */
    auto const res = fwrite(string_builder.as_string_view().as_cstr(), 1, string_builder.len(), file);
    if ( res < string_builder.len() )
        return Error::construct_from_errno(static_cast<ErrnoCode>(ferror(stdout)));
    else
        return {};
}

template<typename... TArgs>
auto voutln(FILE* file, StringView format_view, TArgs&&... args) -> ErrorOr<void> {
    TRY(vout(file, format_view, Cxx::forward<TArgs>(args)...));
    TRY(vout(file, "\n"sv));
    return {};
}

} /* namespace FmtIO */