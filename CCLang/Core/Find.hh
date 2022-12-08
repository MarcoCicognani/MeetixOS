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
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

namespace Details {

auto bitwise_find_in_memory(Slice<u8 const> haystack, Slice<u8 const> needle) -> Option<usize> {
    usize needle_mask[256];
    auto needle_mask_slice = Slice<usize>::from_raw_parts(needle_mask, 256);
    for ( auto const i : usize::range(0, 256) ) {
        needle_mask_slice[i] = 0xffffffff;
    }

    for ( auto const i : usize::range(0, needle.len()) ) {
        needle_mask_slice[needle[i].as<usize>()] &= ~(0x00000001 << i);
    }

    usize lookup = 0xfffffffe;
    for ( auto const i : usize::range(0, haystack.len()) ) {
        lookup |= needle_mask_slice[haystack[i].as<usize>()];
        lookup <<= 1;

        if ( (lookup & (0x00000001 << needle.len())) == 0 ) {
            return i - needle.len() + 1;
        }
    }
    return {};
}

auto kmp_find_in_memory(Slice<u8 const> haystack, Slice<u8 const> needle) -> Option<usize> {
    auto prepare_kmp_partial_table = [&] -> ErrorOr<Vector<i32>> {
        auto kmp_partial_table = // try$(Vector<i32>::try_with_capacity(needle.len()));
        ({
            auto __tryable = (Vector<i32>::try_with_capacity(needle.len()));
            if ( __tryable.__is_bad_variant() ) [[unlikely]] {
                return __tryable.__propagate_failure();
            }
            __tryable.unwrap();
        });

        usize position       = 1;
        i32   candidate      = 0;
        kmp_partial_table[0] = -1;
        while ( position < needle.len() ) {
            if ( needle[position] == needle[candidate.as<usize>()] ) {
                kmp_partial_table[position] = kmp_partial_table[candidate.as<usize>()];
            } else {
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
    if ( error_or_partial_table.is_error() ) {
        return {};
    }

    auto const kmp_partial_table = error_or_partial_table.unwrap();

    usize current_haystack_index = 0;
    i32   needle_index           = 0;

    while ( current_haystack_index < haystack_len ) {
        if ( needle[needle_index.as<usize>()] == haystack[current_haystack_index] ) {
            ++needle_index;
            ++current_haystack_index;
            if ( needle_index.as<usize>() == needle_len ) {
                return current_haystack_index - needle_index.as<usize>();
            } else {
                continue;
            }
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
        if ( Cxx::memcmp(haystack, needle, haystack_len) == Order::Equal ) {
            return usize(0);
        } else {
            return {};
        }
    }

    if ( needle_len < 32 ) {
        return Details::bitwise_find_in_memory(haystack, haystack_len, needle, needle_len);
    } else {
        return Details::kmp_find_in_memory(haystack, haystack_len, needle, needle_len);
    }
}
