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

#include <CCLang/Alloc/Text/FormatLexer.hh>
#include <CCLang/Core/CharTypes.hh>

auto FormatLexer::from_view(StringView source_view) -> FormatLexer {
    return FormatLexer(source_view);
}

auto FormatLexer::consume_literal() -> StringView {
    auto const begin_index = index();
    while ( !is_end() ) {
        if ( consume_specific("{{"sv) )
            continue;
        if ( consume_specific("}}"sv) )
            continue;
        if ( next_is(is_any_of("{}"sv)) )
            return source_view().sub_string_view(begin_index, index() - begin_index);

        consume();
    }
    return source_view().sub_string_view(begin_index);
}

auto FormatLexer::consume_number(usize& value) -> bool {
    value = 0;

    bool consumed_at_least_one = false;
    while ( next_is(is_ascii_digit) ) {
        value *= 10;
        value += consume() - '0';
        consumed_at_least_one = true;
    }
    return consumed_at_least_one;
}

FormatLexer::FormatLexer(StringView source_view)
    : Lexer(source_view) {
}
