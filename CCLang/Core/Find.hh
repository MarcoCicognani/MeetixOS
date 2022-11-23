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

#include <LibC/string.h>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>

template<typename T>
constexpr auto find_in_memory(T const* haystack, usize haystack_count, T const* needle, usize needle_count) -> Option<T const*> {

    /* TODO Implement memmem into CCLang.Core */

    //auto result = Cxx::bit_cast<T const*>(memmem(haystack, haystack_count / sizeof(T), needle, needle_count / sizeof(T)));
    //if ( result != nullptr )
        //return result;
    //else
        return {};
}
