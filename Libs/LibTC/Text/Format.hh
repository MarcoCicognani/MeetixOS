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

#include <LibTC/Collection/StringBuilder.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Text/FormatLexer.hh>
#include <LibTC/Text/FormatParser.hh>
#include <LibTC/Text/Formatter.hh>

namespace TC {
namespace Text {

/**
 * @brief Writes the given literals_view into StringBuilder
 */
ErrorOr<void> format(StringBuilder& string_builder, StringView literals_view);

/**
 * @brief Formats the given StringView and writes out into the given StringBuilder
 */
template<typename... Args>
ErrorOr<void> format(StringBuilder& string_builder, StringView format_view, Args&&... variadic_args) {
    FormatLexer format_lexer{ format_view };
    return format(string_builder, format_lexer, forward<Args>(variadic_args)...);
}

/**
 * @brief Formats first_arg according to the format_lexer into StringBuilder and forward recurse to itself until
 * variadic_args are > 0
 */
template<typename T, typename... Args>
ErrorOr<void> format(StringBuilder& string_builder, FormatLexer& format_lexer, T first_arg, Args&&... variadic_args) {
    /* consume all the non format literals */
    TRY(format(string_builder, format_lexer.consume_literal()));

    /* format the value at the placeholder */
    if ( format_lexer.next_is('{') ) {
        FormatParser format_parser{ format_lexer };
        Formatter<T> formatter{ string_builder, TRY(format_parser.try_parse()) };

        /* try to format the argument value using the Formatter implementation for the type */
        TRY(formatter.format(first_arg));
    }

    /* forward recursion */
    if ( !format_lexer.is_end() )
        TRY(format(string_builder, format_lexer, forward<Args>(variadic_args)...));
    return {};
}

ErrorOr<void> format(StringBuilder& string_builder, FormatLexer& format_lexer);

} /* namespace Text */

using Text::format;

} /* namespace TC */
