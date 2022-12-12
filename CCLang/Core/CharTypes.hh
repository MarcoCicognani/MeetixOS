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

#pragma once

#include <CCLang/Forward.hh>

#include <CCLang/Lang/IntTypes.hh>

using UTF8Rune = u32;

auto is_ascii(UTF8Rune) -> bool;
auto is_ascii_digit(UTF8Rune) -> bool;
auto is_ascii_uppercase_alpha(UTF8Rune) -> bool;
auto is_ascii_lowercase_alpha(UTF8Rune) -> bool;
auto is_ascii_alpha(UTF8Rune) -> bool;
auto is_ascii_alphanumeric(UTF8Rune) -> bool;
auto is_ascii_binary_digit(UTF8Rune) -> bool;
auto is_ascii_octal_digit(UTF8Rune) -> bool;
auto is_ascii_hex_digit(UTF8Rune) -> bool;
auto is_ascii_int_sign(UTF8Rune) -> bool;
auto is_ascii_blank(UTF8Rune) -> bool;
auto is_ascii_space(UTF8Rune) -> bool;
auto is_ascii_punctuation(UTF8Rune) -> bool;
auto is_ascii_graphical(UTF8Rune) -> bool;
auto is_ascii_printable(UTF8Rune) -> bool;
auto is_ascii_c0_control(UTF8Rune) -> bool;
auto is_ascii_control(UTF8Rune) -> bool;

auto is_unicode(UTF8Rune) -> bool;
auto is_unicode_control(UTF8Rune) -> bool;
auto is_unicode_surrogate(UTF8Rune) -> bool;
auto is_unicode_scalar_value(UTF8Rune) -> bool;
auto is_unicode_non_character(UTF8Rune) -> bool;

auto to_ascii_lowercase(UTF8Rune) -> UTF8Rune;
auto to_ascii_uppercase(UTF8Rune) -> UTF8Rune;

auto parse_ascii_digit(UTF8Rune) -> UTF8Rune;
auto parse_ascii_hex_digit(UTF8Rune) -> UTF8Rune;
