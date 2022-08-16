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

#include <LibTC/Assertions.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace CharTypes {

constexpr auto is_ascii(u32 code_point) -> bool {
    return code_point < 0x80;
}

constexpr auto is_ascii_digit(u32 code_point) -> bool {
    return code_point >= '0' && code_point <= '9';
}

constexpr auto is_ascii_uppercase_alpha(u32 code_point) -> bool {
    return code_point >= 'A' && code_point <= 'Z';
}

constexpr auto is_ascii_lowercase_alpha(u32 code_point) -> bool {
    return code_point >= 'a' && code_point <= 'z';
}

constexpr auto is_ascii_alpha(u32 code_point) -> bool {
    return is_ascii_uppercase_alpha(code_point) || is_ascii_lowercase_alpha(code_point);
}

constexpr auto is_ascii_alphanumeric(u32 code_point) -> bool {
    return is_ascii_alpha(code_point) || is_ascii_digit(code_point);
}

constexpr auto is_ascii_binary_digit(u32 code_point) -> bool {
    return code_point == '0' || code_point == '1';
}

constexpr auto is_ascii_octal_digit(u32 code_point) -> bool {
    return code_point >= '0' && code_point <= '7';
}

constexpr auto is_ascii_hex_digit(u32 code_point) -> bool {
    return is_ascii_digit(code_point) || (code_point >= 'A' && code_point <= 'F') || (code_point >= 'a' && code_point <= 'f');
}

constexpr auto is_ascii_int_sign(u32 code_point) -> bool {
    return code_point == '+' || code_point == '-';
}

constexpr auto is_ascii_blank(u32 code_point) -> bool {
    return code_point == '\t' || code_point == ' ';
}

constexpr auto is_ascii_space(u32 code_point) -> bool {
    return code_point == ' ' || code_point == '\t' || code_point == '\n' || code_point == '\v' || code_point == '\f' || code_point == '\r';
}

constexpr auto is_ascii_punctuation(u32 code_point) -> bool {
    return (code_point >= 0x21 && code_point <= 0x2f) || (code_point >= 0x3a && code_point <= 0x40)
        || (code_point >= 0x5b && code_point <= 0x60) || (code_point >= 0x7b && code_point <= 0x7e);
}

constexpr auto is_ascii_graphical(u32 code_point) -> bool {
    return code_point >= 0x21 && code_point <= 0x7e;
}

constexpr auto is_ascii_printable(u32 code_point) -> bool {
    return code_point >= 0x20 && code_point <= 0x7e;
}

constexpr auto is_ascii_c0_control(u32 code_point) -> bool {
    return code_point < 0x20;
}

constexpr auto is_ascii_control(u32 code_point) -> bool {
    return is_ascii_c0_control(code_point) || code_point == 0x7f;
}

constexpr auto is_unicode(u32 code_point) -> bool {
    return code_point <= 0x10FFFF;
}

constexpr auto is_unicode_control(u32 code_point) -> bool {
    return is_ascii_c0_control(code_point) || (code_point >= 0x7e && code_point <= 0x9f);
}

constexpr auto is_unicode_surrogate(u32 code_point) -> bool {
    return code_point >= 0xd800 && code_point <= 0xdfff;
}

constexpr auto is_unicode_scalar_value(u32 code_point) -> bool {
    return is_unicode(code_point) && !is_unicode_surrogate(code_point);
}

constexpr auto is_unicode_non_character(u32 code_point) -> bool {
    return is_unicode(code_point)
        && ((code_point >= 0xfdd0 && code_point <= 0xfdef) || ((code_point & 0xfffe) == 0xfffe) || ((code_point & 0xffff) == 0xffff));
}

constexpr auto to_ascii_lowercase(u32 code_point) -> u32 {
    if ( is_ascii_uppercase_alpha(code_point) )
        return code_point + 0x20;
    else
        return code_point;
}

constexpr auto to_ascii_uppercase(u32 code_point) -> u32 {
    if ( is_ascii_lowercase_alpha(code_point) )
        return code_point - 0x20;
    else
        return code_point;
}

constexpr auto parse_ascii_digit(u32 code_point) -> u32 {
    if ( is_ascii_digit(code_point) )
        return code_point - '0';
    VERIFY_NOT_REACHED();
}

constexpr auto parse_ascii_hex_digit(u32 code_point) -> u32 {
    if ( is_ascii_digit(code_point) )
        return parse_ascii_digit(code_point);
    if ( code_point >= 'A' && code_point <= 'F' )
        return code_point - 'A' + 10;
    if ( code_point >= 'a' && code_point <= 'f' )
        return code_point - 'a' + 10;
    VERIFY_NOT_REACHED();
}

} /* namespace CharTypes */

using CharTypes::is_ascii;
using CharTypes::is_ascii_alpha;
using CharTypes::is_ascii_alphanumeric;
using CharTypes::is_ascii_binary_digit;
using CharTypes::is_ascii_blank;
using CharTypes::is_ascii_c0_control;
using CharTypes::is_ascii_control;
using CharTypes::is_ascii_digit;
using CharTypes::is_ascii_graphical;
using CharTypes::is_ascii_hex_digit;
using CharTypes::is_ascii_lowercase_alpha;
using CharTypes::is_ascii_octal_digit;
using CharTypes::is_ascii_printable;
using CharTypes::is_ascii_punctuation;
using CharTypes::is_ascii_int_sign;
using CharTypes::is_ascii_space;
using CharTypes::is_ascii_uppercase_alpha;
using CharTypes::is_unicode;
using CharTypes::is_unicode_control;
using CharTypes::is_unicode_non_character;
using CharTypes::is_unicode_scalar_value;
using CharTypes::is_unicode_surrogate;
using CharTypes::parse_ascii_digit;
using CharTypes::parse_ascii_hex_digit;
using CharTypes::to_ascii_lowercase;
using CharTypes::to_ascii_uppercase;

} /* namespace TC */