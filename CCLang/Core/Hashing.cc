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

#include <CCLang/Core/Hashing.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Range.hh>

auto Hashing::hash_u32(u32 key) -> usize {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

auto Hashing::hash_u64(u64 key) -> usize {
    u32 first = key & 0xffffffff;
    u32 last  = key >> 32;
    return hash_u32((hash_u32(first) * 209) ^ (hash_u32(last * 413)));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

auto Hashing::hash_ptr(void const* key) -> usize {
    if constexpr ( sizeof(void const*) == 4 )
        return hash_u32(Cxx::bit_cast<u32>(key));
    else
        return hash_u64(Cxx::bit_cast<u64>(key));
}

#pragma clang diagnostic pop

auto Hashing::hash_string(char const* key, usize len) -> usize {
    usize hash = 0;
    for ( auto const i : usize::range(0, len) ) {
        hash += static_cast<u32>(key[i]);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

auto Hashing::hash_again(u32 key) -> usize {
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
