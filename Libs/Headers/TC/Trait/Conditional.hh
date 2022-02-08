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
namespace Trait {
namespace Details {

template<bool condition, typename TrueType, typename FalseType>
struct Conditional {
    using Type = TrueType;
};

template<typename TrueType, typename FalseType>
struct Conditional<false, TrueType, FalseType> {
    using Type = FalseType;
};

} /* namespace Details */

template<bool condition, typename TrueType, typename FalseType>
using Conditional = typename Details::Conditional<condition, TrueType, FalseType>::Type;

} /* namespace Trait */

using Trait::Conditional;

} /* namespace TC */