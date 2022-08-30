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

#include <LibC/string.h>
#include <LibTC/BitCast.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Memory {

template<typename T>
constexpr auto find_in_memory(T const* haystack, usize haystack_count, T const* needle, usize needle_count) -> Option<T const*> {
    auto result = bit_cast<T const*>(memmem(haystack, haystack_count / sizeof(T), needle, needle_count / sizeof(T)));
    if ( result != nullptr )
        return result;
    else
        return {};
}

} /* namespace Memory */

using Memory::find_in_memory;

} /* namespace TC */