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

#include <ST/Alloc/StringBuilder.hh>
#include <ST/Alloc/Text/Format.hh>
#include <ST/Core/ErrorOr.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/StringView.hh>
#include <ST/Lang/Try.hh>

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
    try$(format(string_builder, format_view, Cxx::forward<TArgs>(args)...));

    /* write the result into the stdout */
    auto const res = fwrite(string_builder.as_string_view().as_cstr(), 1, string_builder.len(), file);
    if ( res < string_builder.len() )
        return Error::construct_from_code(static_cast<ErrnoCode>(ferror(stdout)));
    else
        return {};
}

template<typename... TArgs>
auto voutln(FILE* file, StringView format_view, TArgs&&... args) -> ErrorOr<void> {
    try$(vout(file, format_view, Cxx::forward<TArgs>(args)...));
    try$(vout(file, "\n"sv));
    return {};
}

} /* namespace FmtIO */