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

#include <CCLang/Core/CharTypes.hh>
#include <CCLang/Core/Assertions.hh>

auto is_ascii(UTF8Rune code_point) -> bool {
    return code_point < 0x80;
}

auto is_ascii_digit(UTF8Rune code_point) -> bool {
    return code_point >= '0' && code_point <= '9';
}

auto is_ascii_uppercase_alpha(UTF8Rune code_point) -> bool {
    return code_point >= 'A' && code_point <= 'Z';
}

auto is_ascii_lowercase_alpha(UTF8Rune code_point) -> bool {
    return code_point >= 'a' && code_point <= 'z';
}

auto is_ascii_alpha(UTF8Rune code_point) -> bool {
    return is_ascii_uppercase_alpha(code_point) || is_ascii_lowercase_alpha(code_point);
}

auto is_ascii_alphanumeric(UTF8Rune code_point) -> bool {
    return is_ascii_alpha(code_point) || is_ascii_digit(code_point);
}

auto is_ascii_binary_digit(UTF8Rune code_point) -> bool {
    return code_point == '0' || code_point == '1';
}

auto is_ascii_octal_digit(UTF8Rune code_point) -> bool {
    return code_point >= '0' && code_point <= '7';
}

auto is_ascii_hex_digit(UTF8Rune code_point) -> bool {
    return is_ascii_digit(code_point) || (code_point >= 'A' && code_point <= 'F') || (code_point >= 'a' && code_point <= 'f');
}

auto is_ascii_int_sign(UTF8Rune code_point) -> bool {
    return code_point == '+' || code_point == '-';
}

auto is_ascii_blank(UTF8Rune code_point) -> bool {
    return code_point == '\t' || code_point == ' ';
}

auto is_ascii_space(UTF8Rune code_point) -> bool {
    return code_point == ' ' || code_point == '\t' || code_point == '\n' || code_point == '\v' || code_point == '\f' || code_point == '\r';
}

auto is_ascii_punctuation(UTF8Rune code_point) -> bool {
    return (code_point >= 0x21 && code_point <= 0x2f) || (code_point >= 0x3a && code_point <= 0x40) || (code_point >= 0x5b && code_point <= 0x60)
        || (code_point >= 0x7b && code_point <= 0x7e);
}

auto is_ascii_graphical(UTF8Rune code_point) -> bool {
    return code_point >= 0x21 && code_point <= 0x7e;
}

auto is_ascii_printable(UTF8Rune code_point) -> bool {
    return code_point >= 0x20 && code_point <= 0x7e;
}

auto is_ascii_c0_control(UTF8Rune code_point) -> bool {
    return code_point < 0x20;
}

auto is_ascii_control(UTF8Rune code_point) -> bool {
    return is_ascii_c0_control(code_point) || code_point == 0x7f;
}

auto is_unicode(UTF8Rune code_point) -> bool {
    return code_point <= 0x10FFFF;
}

auto is_unicode_control(UTF8Rune code_point) -> bool {
    return is_ascii_c0_control(code_point) || (code_point >= 0x7e && code_point <= 0x9f);
}

auto is_unicode_surrogate(UTF8Rune code_point) -> bool {
    return code_point >= 0xd800 && code_point <= 0xdfff;
}

auto is_unicode_scalar_value(UTF8Rune code_point) -> bool {
    return is_unicode(code_point) && !is_unicode_surrogate(code_point);
}

auto is_unicode_non_character(UTF8Rune code_point) -> bool {
    return is_unicode(code_point) && ((code_point >= 0xfdd0 && code_point <= 0xfdef) || ((code_point & 0xfffe) == 0xfffe) || ((code_point & 0xffff) == 0xffff));
}

auto to_ascii_lowercase(UTF8Rune code_point) -> UTF8Rune {
    if ( is_ascii_uppercase_alpha(code_point) )
        return code_point + 0x20;
    else
        return code_point;
}

auto to_ascii_uppercase(UTF8Rune code_point) -> UTF8Rune {
    if ( is_ascii_lowercase_alpha(code_point) )
        return code_point - 0x20;
    else
        return code_point;
}

auto parse_ascii_digit(UTF8Rune code_point) -> UTF8Rune {
    if ( is_ascii_digit(code_point) )
        return code_point - '0';
    verify_not_reached$();
}

auto parse_ascii_hex_digit(UTF8Rune code_point) -> UTF8Rune {
    if ( is_ascii_digit(code_point) )
        return parse_ascii_digit(code_point);
    if ( code_point >= 'A' && code_point <= 'F' )
        return code_point - 'A' + 10;
    if ( code_point >= 'a' && code_point <= 'f' )
        return code_point - 'a' + 10;
    verify_not_reached$();
}

