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

#include <LibLS/Alloc/Text/FormatParser.hh>

auto FormatParser::Result::display_as_is_numeric() const -> bool {
    return m_display_as == DisplayAs::Binary
        || m_display_as == DisplayAs::BinaryUpperCase
        || m_display_as == DisplayAs::Octal
        || m_display_as == DisplayAs::Decimal
        || m_display_as == DisplayAs::Hex
        || m_display_as == DisplayAs::HexUpperCase;
}

auto FormatParser::new_from_lexer(FormatLexer& format_lexer) -> FormatParser {
    return FormatParser{ format_lexer };
}

auto FormatParser::try_parse() -> ErrorOr<FormatParser::Result> {
    if ( !m_format_lexer.consume_specific('{') )
        return Error::new_from_code(ErrorCode::Invalid);

    /* parse the format specifiers */
    Result result{};
    if ( m_format_lexer.consume_specific(':') ) {
        parse_alignment_fill(result);
        parse_alignment(result);
        parse_integer_sign(result);
        parse_show_base(result);
        parse_zero_pad(result);
        parse_width(result);

        /* parse the precision specification */
        if ( m_format_lexer.consume_specific('.') ) {
            if ( usize precision; m_format_lexer.consume_number(precision) )
                result.m_precision = precision;
            else
                return Error::new_from_code(ErrorCode::Invalid);
        }

        parse_display_as(result);
    }

    /* parse the termination tag */
    if ( !m_format_lexer.consume_specific('}') )
        return Error::new_from_code(ErrorCode::Invalid);
    else
        return result;
}

auto FormatParser::parse_alignment_fill(FormatParser::Result& result) -> void {
    /* eat the fill character for alignment if the after next character is an alignment specifier */
    if ( "<^>"sv.contains(m_format_lexer.peek(1)) )
        result.m_alignment_fill = m_format_lexer.consume();
}

auto FormatParser::parse_alignment(FormatParser::Result& result) -> void {
    /* parse the alignment specification */
    if ( m_format_lexer.consume_specific('<') )
        result.m_alignment = Alignment::Left;
    else if ( m_format_lexer.consume_specific('^') )
        result.m_alignment = Alignment::Center;
    else if ( m_format_lexer.consume_specific('>') )
        result.m_alignment = Alignment::Right;
}

auto FormatParser::parse_integer_sign(FormatParser::Result& result) -> void {
    /* parse the sign specification */
    if ( m_format_lexer.consume_specific('-') )
        result.m_show_integer_sign = ShowIntegerSign::IfNegative;
    else if ( m_format_lexer.consume_specific('+') )
        result.m_show_integer_sign = ShowIntegerSign::Yes;
    else if ( m_format_lexer.consume_specific(' ') )
        result.m_show_integer_sign = ShowIntegerSign::KeepSpace;
}

auto FormatParser::parse_show_base(FormatParser::Result& result) -> void {
    /* parse the show base specification */
    if ( m_format_lexer.consume_specific('#') )
        result.m_show_base = ShowBase::Yes;
}

auto FormatParser::parse_zero_pad(Result& result) -> void {
    /* parse the zero pad specification */
    if ( m_format_lexer.consume_specific('0') )
        result.m_zero_pad = ZeroPad::Yes;
}

auto FormatParser::parse_width(FormatParser::Result& result) -> void {
    /* parse the max width specification */
    if ( usize width; m_format_lexer.consume_number(width) )
        result.m_width = width;
}

auto FormatParser::parse_display_as(FormatParser::Result& result) -> void {
    /* parse the display as specification */
    if ( m_format_lexer.consume_specific('b') )
        result.m_display_as = DisplayAs::Binary;
    else if ( m_format_lexer.consume_specific('B') )
        result.m_display_as = DisplayAs::BinaryUpperCase;
    else if ( m_format_lexer.consume_specific('o') )
        result.m_display_as = DisplayAs::Octal;
    else if ( m_format_lexer.consume_specific('d') )
        result.m_display_as = DisplayAs::Decimal;
    else if ( m_format_lexer.consume_specific('x') )
        result.m_display_as = DisplayAs::Hex;
    else if ( m_format_lexer.consume_specific('X') )
        result.m_display_as = DisplayAs::HexUpperCase;
    else if ( m_format_lexer.consume_specific('p') )
        result.m_display_as = DisplayAs::Pointer;
    else if ( m_format_lexer.consume_specific('c') )
        result.m_display_as = DisplayAs::Char;
    else if ( m_format_lexer.consume_specific('s') )
        result.m_display_as = DisplayAs::String;
    else if ( m_format_lexer.consume_specific('f') )
        result.m_display_as = DisplayAs::Float;
    else if ( m_format_lexer.consume_specific('a') )
        result.m_display_as = DisplayAs::HexFloat;
    else if ( m_format_lexer.consume_specific('A') )
        result.m_display_as = DisplayAs::HexFloatUpperCase;
}
