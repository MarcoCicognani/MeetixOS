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

#include <LibTC/Text/FormatParser.hh>

namespace TC::Text {

bool FormatParser::Specifications::display_as_is_numeric() const {
    return m_display_as == DisplayAs::Binary || m_display_as == DisplayAs::BinaryUpperCase || m_display_as == DisplayAs::Octal
        || m_display_as == DisplayAs::Decimal || m_display_as == DisplayAs::Hex || m_display_as == DisplayAs::HexUpperCase;
}

FormatParser::FormatParser(FormatLexer& format_lexer)
    : m_format_lexer{ format_lexer } {
}

ErrorOr<FormatParser::Specifications> FormatParser::try_parse() {
    if ( !m_format_lexer.consume_specific('{') )
        return Error{ EINVAL };

    /* parse the format specifiers */
    Specifications specifications{};
    if ( m_format_lexer.consume_specific(':') ) {
        parse_alignment_fill(specifications);
        parse_alignment(specifications);
        parse_integer_sign(specifications);
        parse_show_base(specifications);
        parse_zero_pad(specifications);
        parse_width(specifications);

        /* parse the precision specification */
        if ( m_format_lexer.consume_specific('.') ) {
            if ( usize precision; m_format_lexer.consume_number(precision) )
                specifications.m_precision = precision;
            else
                return Error{ EINVAL };
        }

        parse_display_as(specifications);
    }

    /* parse the termination tag */
    if ( !m_format_lexer.consume_specific('}') )
        return Error{ EINVAL };
    else
        return specifications;
}

void FormatParser::parse_alignment_fill(FormatParser::Specifications& specifications) {
    /* eat the fill character for alignment if the after next character is an alignment specifier */
    if ( "<^>"sv.contains(m_format_lexer.peek(1)) )
        specifications.m_alignment_fill = m_format_lexer.consume();
}

void FormatParser::parse_alignment(FormatParser::Specifications& specifications) {
    /* parse the alignment specification */
    if ( m_format_lexer.consume_specific('<') )
        specifications.m_alignment = Alignment::Left;
    else if ( m_format_lexer.consume_specific('^') )
        specifications.m_alignment = Alignment::Center;
    else if ( m_format_lexer.consume_specific('>') )
        specifications.m_alignment = Alignment::Right;
}

void FormatParser::parse_integer_sign(FormatParser::Specifications& specifications) {
    /* parse the sign specification */
    if ( m_format_lexer.consume_specific('-') )
        specifications.m_show_integer_sign = ShowIntegerSign::IfNegative;
    else if ( m_format_lexer.consume_specific('+') )
        specifications.m_show_integer_sign = ShowIntegerSign::Yes;
    else if ( m_format_lexer.consume_specific(' ') )
        specifications.m_show_integer_sign = ShowIntegerSign::KeepSpace;
}

void FormatParser::parse_show_base(FormatParser::Specifications& specifications) {
    /* parse the show base specification */
    if ( m_format_lexer.consume_specific('#') )
        specifications.m_show_base = ShowBase::Yes;
}

void FormatParser::parse_zero_pad(Specifications& specifications) {
    /* parse the zero pad specification */
    if ( m_format_lexer.consume_specific('0') )
        specifications.m_zero_pad = ZeroPad::Yes;
}

void FormatParser::parse_width(FormatParser::Specifications& specifications) {
    /* parse the max width specification */
    if ( usize width; m_format_lexer.consume_number(width) )
        specifications.m_width = width;
}

void FormatParser::parse_display_as(FormatParser::Specifications& specifications) {
    /* parse the display as specification */
    if ( m_format_lexer.consume_specific('b') )
        specifications.m_display_as = DisplayAs::Binary;
    else if ( m_format_lexer.consume_specific('B') )
        specifications.m_display_as = DisplayAs::BinaryUpperCase;
    else if ( m_format_lexer.consume_specific('o') )
        specifications.m_display_as = DisplayAs::Octal;
    else if ( m_format_lexer.consume_specific('d') )
        specifications.m_display_as = DisplayAs::Decimal;
    else if ( m_format_lexer.consume_specific('x') )
        specifications.m_display_as = DisplayAs::Hex;
    else if ( m_format_lexer.consume_specific('X') )
        specifications.m_display_as = DisplayAs::HexUpperCase;
    else if ( m_format_lexer.consume_specific('p') )
        specifications.m_display_as = DisplayAs::Pointer;
    else if ( m_format_lexer.consume_specific('c') )
        specifications.m_display_as = DisplayAs::Char;
    else if ( m_format_lexer.consume_specific('s') )
        specifications.m_display_as = DisplayAs::String;
    else if ( m_format_lexer.consume_specific('f') )
        specifications.m_display_as = DisplayAs::Float;
    else if ( m_format_lexer.consume_specific('a') )
        specifications.m_display_as = DisplayAs::HexFloat;
    else if ( m_format_lexer.consume_specific('A') )
        specifications.m_display_as = DisplayAs::HexFloatUpperCase;
}

} /* namespace TC::Text */