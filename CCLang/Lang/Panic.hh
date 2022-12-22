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

#include <CCLang/Lang/StringView.hh>

[[noreturn]]
auto panic(StringView msg, StringView file = StringView::from_cstr(__builtin_FILE()), usize line = __builtin_LINE()) -> void;