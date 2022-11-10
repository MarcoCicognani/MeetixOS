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

#include <LibLS/Core/Assertions.hh>

#ifdef IN_KERNEL

auto Details::__assert_plug_panic(char const*, char const*, u32) -> void {
    __builtin_unreachable();
}

#else

#    include <LibRT/RT.hh>

auto Details::__assert_plug_panic(char const*, char const*, u32) -> void {
    __builtin_unreachable();
}

#endif