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

#include <LibTC/Collection/Range.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/BitCast.hh>

namespace TC::Hashing {

usize u32_calculate_hash(u32 key) {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

usize u64_calculate_hash(u64 key) {
    u32 first = key & 0xffffffff;
    u32 last  = key >> 32;
    return u32_calculate_hash((u32_calculate_hash(first) * 209) ^ (u32_calculate_hash(last * 413)));
}

usize pointer_calculate_hash(void const* key) {
    if constexpr ( sizeof(void const*) == 4 )
        return u32_calculate_hash(bit_cast<u32>(key));
    else
        return u64_calculate_hash(bit_cast<u64>(key));
}

usize string_calculate_hash(char const* key, usize len) {
    u32 hash = 0;
    for ( usize i : Range{ 0uL, len } ) {
        hash += static_cast<u32>(key[i]);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

usize rehash_key(u32 key) {
    auto magic = 0xba5edb01;
    if ( key == magic )
        return 0;
    if ( key == 0 )
        key = magic;

    key ^= key << 13;
    key ^= key >> 17;
    key ^= key << 5;
    return key;
}

} /* namespace TC::Hashing */