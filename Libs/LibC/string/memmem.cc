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

#include <string.h>
#include <LibTC/Collection/Vector.hh>

static void const* bitwise_memmem(void const* haystack, usize haystack_len, void const* needle, usize needle_len) {
    u64            lookup   = 0xfffffffe;
    constexpr auto mask_len = static_cast<usize>(static_cast<u8>(-1)) + 1;

    u64 needle_mask[mask_len];
    for ( auto& mask : needle_mask )
        mask = 0xffffffff;

    for ( usize i = 0; i < needle_len; ++i )
        needle_mask[static_cast<u8 const*>(needle)[i]] &= ~(0x00000001 << i);

    for ( size_t i = 0; i < haystack_len; ++i ) {
        lookup |= needle_mask[static_cast<u8 const*>(haystack)[i]];
        lookup <<= 1;

        if ( (lookup & (0x00000001 << needle_len)) == 0 )
            return static_cast<u8 const*>(haystack) + i - needle_len + 1;
    }
    return nullptr;
}

static void const* kmp_memmem(u8 const* haystack, usize haystack_len, u8 const* needle, usize needle_len) {
    auto prepare_kmp_partial_table = [&] {
        TC::Vector<int> partial_table{};
        partial_table.resize(needle_len);

        usize position   = 1;
        int   candidate  = 0;
        partial_table[0] = -1;
        while ( position < needle_len ) {
            if ( needle[position] == needle[candidate] )
                partial_table[position] = partial_table[candidate];
            else {
                partial_table[position] = candidate;
                do {
                    candidate = partial_table[candidate];
                } while ( candidate >= 0 && needle[candidate] != needle[position] );
            }

            ++position;
            ++candidate;
        }
        return partial_table;
    };

    auto  partial_table          = prepare_kmp_partial_table();
    usize current_haystack_index = 0;
    int   needle_index           = 0;

    while ( current_haystack_index < haystack_len ) {
        if ( needle[needle_index] == haystack[current_haystack_index] ) {
            ++needle_index;
            ++current_haystack_index;
            if ( static_cast<usize>(needle_index) == needle_len )
                return reinterpret_cast<void const*>(haystack + needle_index);
            else
                continue;
        }

        needle_index = partial_table[needle_index];
        if ( needle_index < 0 ) {
            ++needle_index;
            ++current_haystack_index;
        }
    }
    return nullptr;
}

extern "C" const void* memmem(const void* haystack, usize haystack_len, const void* needle, usize needle_len) {
    if ( needle_len == 0 )
        return haystack;

    if ( haystack_len < needle_len )
        return nullptr;

    if ( haystack_len == needle_len ) {
        if ( memcmp(haystack, needle, haystack_len) == 0 )
            return haystack;
        else
            return nullptr;
    }

    if ( needle_len < 32 )
        return bitwise_memmem(haystack, haystack_len, needle, needle_len);
    else
        return kmp_memmem(reinterpret_cast<u8 const*>(haystack),
                          haystack_len,
                          reinterpret_cast<u8 const*>(needle),
                          needle_len);
}