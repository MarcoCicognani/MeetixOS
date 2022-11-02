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

#include <LibTC/Lang/IntTypes.hh>

namespace Hashing {

/**
 * @brief Calculates the hash value for the given u32 key
 */
auto u32_calculate_hash(u32 key) -> usize;

/**
 * @brief Calculates the hash value for the given u64 key
 */
auto u64_calculate_hash(u64 key) -> usize;

/**
 * @brief Calculates the hash value for the given pointer
 */
auto pointer_calculate_hash(void const* key) -> usize;

/**
 * @brief Calculates the hash value for the given string buffer
 */
auto string_calculate_hash(char const* key, usize len) -> usize;

/**
 * @brief Calculates another level of hashing for the given value
 */
auto rehash_key(u32 key) -> usize;

} /* namespace Hashing */
