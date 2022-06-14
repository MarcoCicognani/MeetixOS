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

#include <LibTC/IntTypes.hh>

namespace TC {
namespace Hashing {

/**
 * @brief Calculates the hash value for the given u32 key
 */
usize u32_calculate_hash(u32 key);

/**
 * @brief Calculates the hash value for the given u64 key
 */
usize u64_calculate_hash(u64 key);

/**
 * @brief Calculates the hash value for the given pointer
 */
usize pointer_calculate_hash(void const* key);

/**
 * @brief Calculates the hash value for the given string buffer
 */
usize string_calculate_hash(char const* key, usize len);

/**
 * @brief Calculates another level of hashing for the given value
 */
usize rehash_key(u32 key);

} /* namespace Hashing */

using Hashing::pointer_calculate_hash;
using Hashing::rehash_key;
using Hashing::string_calculate_hash;
using Hashing::u32_calculate_hash;
using Hashing::u64_calculate_hash;

} /* namespace TC */
