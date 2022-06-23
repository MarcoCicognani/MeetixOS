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

namespace TC {
namespace BitCast {

template<typename T, typename U>
constexpr T bit_cast(U const& from) {
    return __builtin_bit_cast(T, from);
}

} /* namespace BitCast */

using BitCast::bit_cast;

} /* namespace TC */