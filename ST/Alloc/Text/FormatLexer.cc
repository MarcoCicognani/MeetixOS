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

#include <ST/Alloc/Text/FormatLexer.hh>
#include <ST/Core/CharTypes.hh>

auto FormatLexer::consume_literal() -> StringView {
    auto begin = index();
    while ( !is_end() ) {
        if ( consume_specific("{{"sv) )
            continue;
        if ( consume_specific("}}"sv) )
            continue;
        if ( next_is(is_any_of("{}"sv)) )
            return source_view().sub_string_view(begin, index() - begin);

        consume();
    }
    return source_view().sub_string_view(begin);
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
