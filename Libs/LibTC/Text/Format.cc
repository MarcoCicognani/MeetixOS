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

ErrorOr<void> format(StringBuilder& string_builder, StringView literals_view) {
    for ( usize i = 0; i < literals_view.len(); ++i ) {
        TRY(string_builder.try_append(literals_view[i]));

        /* skip escaped placeholders */
        if ( (literals_view[i] == '{' && i + 1 < literals_view.len() && literals_view[i + 1] == '{')
             || (literals_view[i] == '}' && i + 1 < literals_view.len() && literals_view[i + 1] == '}') )
            ++i;
    }
    return {};
}

ErrorOr<void> format(StringBuilder& string_builder, FormatLexer& format_lexer) {
    return format(string_builder, format_lexer.consume_all());
}

} /* namespace TC::Text */