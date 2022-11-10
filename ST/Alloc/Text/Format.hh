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

#include <ST/Forward.hh>

#include <ST/Alloc/StringBuilder.hh>
#include <ST/Alloc/Text/FormatLexer.hh>
#include <ST/Alloc/Text/FormatParser.hh>
#include <ST/Alloc/Text/Formatter.hh>
#include <ST/Core/ErrorOr.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/IntTypes.hh>
#include <ST/Lang/StringView.hh>
#include <ST/Lang/Try.hh>

/**
 * @brief Writes the given literals_view into StringBuilder
 */
auto format(StringBuilder& string_builder, StringView literals_view) -> ErrorOr<void>;

/**
 * @brief Formats the given StringView and writes out into the given StringBuilder
 */
template<typename... Args>
auto format(StringBuilder& string_builder, StringView format_view, Args&&... variadic_args) -> ErrorOr<void> {
    auto format_lexer = FormatLexer::new_from_view(format_view);
    try$(format(string_builder, format_lexer, Cxx::forward<Args>(variadic_args)...));
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
        try$(format(string_builder, literals));

    /* format the value at the placeholder */
    if ( format_lexer.next_is('{') ) {
        auto format_parser = FormatParser::new_from_lexer(format_lexer);
        auto formatter     = Formatter<T>::new_from_parser_result(string_builder, try$(format_parser.try_parse()));

        /* try to format the argument value using the Formatter implementation for the type */
        try$(formatter.format(first_arg));
    }

    /* forward recursion */
    if ( !format_lexer.is_end() )
        try$(format(string_builder, format_lexer, Cxx::forward<Args>(variadic_args)...));
    return {};
}

auto format(StringBuilder& string_builder, FormatLexer& format_lexer) -> ErrorOr<void>;
