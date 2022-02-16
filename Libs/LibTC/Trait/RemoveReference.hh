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
struct RemoveReference {
    using Type = T;
};

template<typename T>
struct RemoveReference<T&> {
    using Type = T;
};

template<typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

} /* namespace Details */

template<typename T>
using RemoveReference = typename Details::RemoveReference<T>::Type;

} /* namespace Trait */

using Trait::RemoveReference;

} /* namespace TC */