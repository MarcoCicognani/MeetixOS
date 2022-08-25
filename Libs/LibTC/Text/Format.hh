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

#include <LibTC/Forward.hh>

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
auto format(StringBuilder& string_builder, StringView literals_view) -> ErrorOr<void>;

/**
 * @brief Formats the given StringView and writes out into the given StringBuilder
 */
template<typename... Args>
auto format(StringBuilder& string_builder, StringView format_view, Args&&... variadic_args) -> ErrorOr<void> {
    auto format_lexer = FormatLexer::construct_from_view(format_view);
    TRY(format(string_builder, format_lexer, Cxx::forward<Args>(variadic_args)...));
    return {};
}

/**
 * @brief Formats first_arg according to the format_lexer into StringBuilder and forward recurse to itself until
 * variadic_args are > 0
 */
template<typename T, typename... Args>
auto format(StringBuilder& string_builder, FormatLexer& format_lexer, T const& first_arg, Args&&... variadic_args) -> ErrorOr<void> {
    /* consume all the non format literals */
    if ( auto literals = format_lexer.consume_literal(); !literals.is_null_or_empty() )
        TRY(format(string_builder, literals));

    /* format the value at the placeholder */
    if ( format_lexer.next_is('{') ) {
        auto format_parser = FormatParser::construct_from_lexer(format_lexer);
        auto formatter     = Formatter<T>::construct_from_parser_result(string_builder, TRY(format_parser.try_parse()));

        /* try to format the argument value using the Formatter implementation for the type */
        TRY(formatter.format(first_arg));
    }

    /* forward recursion */
    if ( !format_lexer.is_end() )
        TRY(format(string_builder, format_lexer, Cxx::forward<Args>(variadic_args)...));
    return {};
}

auto format(StringBuilder& string_builder, FormatLexer& format_lexer) -> ErrorOr<void>;

} /* namespace Text */

using Text::format;

} /* namespace TC */
