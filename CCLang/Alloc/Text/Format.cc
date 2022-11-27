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

#include <CCLang/Alloc/Text/Format.hh>
#include <CCLang/Lang/Try.hh>

auto format(StringBuilder& string_builder, StringView literals_view) -> ErrorOr<void> {
    auto const is_escaped_placeholder = [](StringView sv) { return sv.starts_with("{{"sv) || sv.starts_with("}}"sv); };

    usize i = 0;
    while ( i < literals_view.len() ) {
        try$(string_builder.try_append(literals_view[i]));

        /* skip escaped placeholders */
        if ( is_escaped_placeholder(literals_view.sub_string_view(i)) )
            i += 2;
        else
            i += 1;
    }
    return {};
}

auto format(StringBuilder& string_builder, FormatLexer& format_lexer) -> ErrorOr<void> {
    try$(format(string_builder, format_lexer.consume_all()));
    return {};
}
