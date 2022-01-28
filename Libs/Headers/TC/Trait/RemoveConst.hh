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

namespace TC::Trait {
namespace Details {

template<typename T>
struct RemoveConst {
    using Type = T;
};

template<typename T>
struct RemoveConst<T const> {
    using Type = T;
};

} /* namespace Details */

template<typename T>
using RemoveConst = typename Details::RemoveConst<T>::Type;

} /* namespace TC::Trait */