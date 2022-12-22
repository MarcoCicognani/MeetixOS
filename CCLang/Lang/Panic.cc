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

#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Panic.hh>
#include <CCLang/Lang/StringView.hh>

extern "C++" {

[[noreturn]]
auto __rt_panic_plugin(StringView msg, StringView file, usize line) -> void;

} /* extern "C++" */

[[noreturn]]
auto panic(StringView msg, StringView file, usize line) -> void {
    __rt_panic_plugin(msg, file, line);
}