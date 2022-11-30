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

#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/Assertions.hh>
#include <CCLang/Lang/Array.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

namespace Details {

auto bitwise_find_in_memory(UnsafeArrayPtr<u8 const> haystack, usize haystack_len, UnsafeArrayPtr<u8 const> needle, usize needle_len) -> Option<usize> {
    UnsafeInlineArray<usize, 256> needle_mask;
    for ( auto const i : usize::range(0, 256) )
        needle_mask[i] = 0xffffffff;

    for ( auto const i : usize::range(0, needle_len) ) {
        needle_mask[needle[i].as<usize>()] &= ~(0x00000001 << i);
    }

    usize lookup = 0xfffffffe;
    for ( auto const i : usize::range(0, haystack_len) ) {
        lookup |= needle_mask[haystack[i].as<usize>()];
        lookup <<= 1;

        if ( (lookup & (0x00000001 << needle_len)) == 0 )
            return i - needle_len + 1;
    }
    return {};
}

auto kmp_find_in_memory(UnsafeArrayPtr<u8 const> haystack, usize haystack_len, UnsafeArrayPtr<u8 const> needle, usize needle_len) -> Option<usize> {
    auto prepare_kmp_partial_table = [&] -> ErrorOr<Vector<i32>> {
        auto kmp_partial_table = try$(Vector<i32>::try_with_capacity(needle_len));

        usize position       = 1;
        i32   candidate      = 0;
        kmp_partial_table[0] = -1;
        while ( position < needle_len ) {
            if ( needle[position] == needle[candidate.as<usize>()] )
                kmp_partial_table[position] = kmp_partial_table[candidate.as<usize>()];
            else {
                kmp_partial_table[position] = candidate;
                do {
                    candidate = kmp_partial_table[candidate.as<usize>()];
                } while ( candidate >= 0 && needle[candidate.as<usize>()] != needle[position] );
            }

            ++position;
            ++candidate;
        }
        return kmp_partial_table;
    };

    /* prepare the KMP partial table */
    auto error_or_partial_table = prepare_kmp_partial_table();
    if ( error_or_partial_table.is_error() )
        return {};

    auto const kmp_partial_table = error_or_partial_table.unwrap();

    usize current_haystack_index = 0;
    i32   needle_index           = 0;

    while ( current_haystack_index < haystack_len ) {
        if ( needle[needle_index.as<usize>()] == haystack[current_haystack_index] ) {
            ++needle_index;
            ++current_haystack_index;
            if ( needle_index.as<usize>() == needle_len )
                return current_haystack_index - needle_index.as<usize>();
            else
                continue;
        }

        needle_index = kmp_partial_table[needle_index.as<usize>()];
        if ( needle_index < 0 ) {
            ++needle_index;
            ++current_haystack_index;
        }
    }

    return {};
}

} /* namespace Details */

template<typename T>
auto find_in_memory(T const* haystack, usize haystack_len, T const* needle, usize needle_len) -> Option<usize> {
    if ( needle_len == 0 ) {
        return usize(0);
    }
    if ( haystack_len < needle_len ) {
        return {};
    }
    if ( haystack_len == needle_len ) {
        if ( Cxx::memcmp(haystack, needle, haystack_len) == Order::Equal )
            return usize(0);
        else
            return {};
    }

    if ( needle_len < 32 ) {
        return Details::bitwise_find_in_memory(haystack, haystack_len, needle, needle_len);
    } else {
        return Details::kmp_find_in_memory(haystack, haystack_len, needle, needle_len);
    }
}
