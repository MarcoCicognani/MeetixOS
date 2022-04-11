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

namespace TC {
namespace Trait {

template<typename B, typename D>
inline constexpr bool IsBaseOf = __is_base_of(B, D);

template<typename B, typename D>
concept BaseOf = IsBaseOf<B, D>;

} /* namespace Trait */

using Trait::BaseOf;
using Trait::IsBaseOf;

} /* namespace TC */