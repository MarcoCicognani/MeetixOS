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

#include <LibTC/Hashing/Integer.hh>

namespace TC::Hashing {

usize integer_hash(u64 value) {
    auto u32_hash = [](u32 half_value) -> u32 {
        half_value += ~(half_value << 15);
        half_value ^= (half_value >> 10);
        half_value += (half_value << 3);
        half_value ^= (half_value >> 6);
        half_value += ~(half_value << 11);
        half_value ^= (half_value >> 16);
        return half_value;
    };

    u32 lower = value & 0xffffffff;
    u32 upper = value >> 32;
    return u32_hash((u32_hash(lower) * 209) ^ (u32_hash(upper * 413)));
}

} /* namespace TC::Hashing */