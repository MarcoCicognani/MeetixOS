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
#include <CCLang/Lang/StringView.hh>

/* CCLang.Lang.Panic plugin support */

[[noreturn]]
auto __rt_panic_plugin(StringView, StringView, usize) -> void {
    /* TODO call s_task_exit(...) */
    __builtin_unreachable();
}