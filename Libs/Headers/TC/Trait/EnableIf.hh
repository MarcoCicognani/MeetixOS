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

template<bool B, typename T>
struct EnableIf {
    /* Base */
};

template<typename T>
struct EnableIf<true, T> {
    using Type = T;
};

} /* namespace Details */

template<bool B, typename T>
using EnableIf = typename Details::EnableIf<B, T>::Type;

} /* namespace TC::Trait */