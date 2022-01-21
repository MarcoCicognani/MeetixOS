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

#include <TC/Trait/RemoveConst.hh>
#include <TC/Trait/RemoveVolatile.hh>

namespace TC::Trait {
namespace Details {

template<typename T>
struct RemoveConstVolatile {
    using Type = RemoveVolatile<RemoveConst<T>>;
};

} /* namespace Details */

template<typename T>
using RemoveConstVolatile = typename Details::RemoveConstVolatile<T>::Type;

} /* namespace TC::Trait */