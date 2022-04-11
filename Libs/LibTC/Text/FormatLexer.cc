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

#include <LibC/ctype.h>
#include <LibTC/Text/FormatLexer.hh>

namespace TC::Text {

FormatLexer::FormatLexer(StringView source_view)
    : Lexer(source_view) {
}

StringView FormatLexer::consume_literal() {
    auto begin = index();
    while ( !is_end() ) {
        if ( consume_specific("{{") )
            continue;
        if ( consume_specific("}}") )
            continue;
        if ( next_is(is_any_of("{}")) )
            return source_view().sub_string_view(begin, index() - begin);

        consume();
    }
    return source_view().sub_string_view(begin);
}

bool FormatLexer::consume_number(usize& value) {
    value = 0;

    bool consumed_at_least_one = false;
    while ( next_is(isdigit) ) {
        value *= 10;
        value += consume() - '0';
        consumed_at_least_one = true;
    }
    return consumed_at_least_one;
}

} /* namespace TC::Text */