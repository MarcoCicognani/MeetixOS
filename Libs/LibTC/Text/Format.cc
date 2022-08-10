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

#include <LibTC/Functional/Try.hh>
#include <LibTC/Text/Format.hh>

namespace TC::Text {

auto format(StringBuilder& string_builder, StringView literals_view) -> ErrorOr<void> {
    for ( usize i = 0; i < literals_view.len(); ++i ) {
        TRY(string_builder.try_append(literals_view[i]));

        /* skip escaped placeholders */
        auto const sub_string_view = literals_view.sub_string_view(i);
        if ( sub_string_view.starts_with("{{"sv) || sub_string_view.starts_with("}}"sv) )
            ++i;
    }
    return {};
}

auto format(StringBuilder& string_builder, FormatLexer& format_lexer) -> ErrorOr<void> {
    TRY(format(string_builder, format_lexer.consume_all()));
    return {};
}

} /* namespace TC::Text */