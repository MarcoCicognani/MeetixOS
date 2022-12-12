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

auto is_ascii(u32) -> bool;
auto is_ascii_digit(u32) -> bool;
auto is_ascii_uppercase_alpha(u32) -> bool;
auto is_ascii_lowercase_alpha(u32) -> bool;
auto is_ascii_alpha(u32) -> bool;
auto is_ascii_alphanumeric(u32) -> bool;
auto is_ascii_binary_digit(u32) -> bool;
auto is_ascii_octal_digit(u32) -> bool;
auto is_ascii_hex_digit(u32) -> bool;
auto is_ascii_int_sign(u32) -> bool;
auto is_ascii_blank(u32) -> bool;
auto is_ascii_space(u32) -> bool;
auto is_ascii_punctuation(u32) -> bool;
auto is_ascii_graphical(u32) -> bool;
auto is_ascii_printable(u32) -> bool;
auto is_ascii_c0_control(u32) -> bool;
auto is_ascii_control(u32) -> bool;

auto is_unicode(u32) -> bool;
auto is_unicode_control(u32) -> bool;
auto is_unicode_surrogate(u32) -> bool;
auto is_unicode_scalar_value(u32) -> bool;
auto is_unicode_non_character(u32) -> bool;

auto to_ascii_lowercase(u32) -> u32;
auto to_ascii_uppercase(u32) -> u32;

auto parse_ascii_digit(u32) -> u32;
auto parse_ascii_hex_digit(u32) -> u32;
