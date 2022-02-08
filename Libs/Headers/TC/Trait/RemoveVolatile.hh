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

template<typename T>
struct RemoveVolatile {
    using Type = T;
};

template<typename T>
struct RemoveVolatile<T volatile> {
    using Type = T;
};

} /* namespace Details */

template<typename T>
using RemoveVolatile = typename Details::RemoveVolatile<T>::Type;

} /* namespace Trait */

using Trait::RemoveVolatile;

} /* namespace TC */