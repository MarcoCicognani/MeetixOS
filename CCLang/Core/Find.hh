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

#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>

namespace Details {

auto bitwise_find_in_memory(Slice<u8 const> haystack, Slice<u8 const> needle) -> Option<usize>;
auto kmp_find_in_memory(Slice<u8 const> haystack, Slice<u8 const> needle) -> Option<usize>;

} /* namespace Details */

template<typename T>
auto find_in_memory(Slice<T const> haystack, Slice<T const> needle) -> Option<usize> {
    if ( needle.len() == 0 ) {
        return usize(0);
    }
    if ( haystack.len() < needle.len() ) {
        return {};
    }
    if ( haystack.len() == needle.len() ) {
        if ( Cxx::memcmp(haystack.data(), needle.data(), haystack.len()) == Order::Equal ) {
            return usize(0);
        } else {
            return {};
        }
    }

    if ( needle.len() < 32 ) {
        return Details::bitwise_find_in_memory(haystack.as_slice_of<u8 const>(), needle.as_slice_of<u8 const>());
    } else {
        return Details::kmp_find_in_memory(haystack.as_slice_of<u8 const>(), needle.as_slice_of<u8 const>());
    }
}
