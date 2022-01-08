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

#include <TC/Traits/RemoveConst.hh>
#include <TC/Traits/RemoveVolatile.hh>

namespace TC::Traits {

template<typename T>
struct RemoveConstVolatile {
    using Type = typename RemoveVolatile<typename RemoveConst<T>::Type>::Type;
};

} /* namespace TC::Traits */