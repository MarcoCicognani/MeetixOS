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

#include <CCLang/Core/Assertions.hh>

#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Panic.hh>
#include <CCLang/Lang/StringView.hh>

[[noreturn]]
auto Details::__verify_internal_has_failed(char const* msg, char const* file, int line) -> void {
    panic(StringView::from_cstr(msg), StringView::from_cstr(file), usize(line));
}
