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

#include <LibTC/IntTypes.hh>

namespace TC {
namespace Hashing {

usize integer_hash(u64 value);

} /* namespace Hashing */

using Hashing::integer_hash;

} /* namespace TC */