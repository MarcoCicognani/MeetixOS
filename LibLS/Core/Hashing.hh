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

#include <LibLS/Forward.hh>

#include <LibLS/Lang/IntTypes.hh>

namespace Hashing {

auto hash_u32(u32 key) -> usize;
auto hash_u64(u64 key) -> usize;
auto hash_ptr(void const* key) -> usize;
auto hash_string(char const* key, usize len) -> usize;
auto hash_again(u32 key) -> usize;

} /* namespace Hashing */
