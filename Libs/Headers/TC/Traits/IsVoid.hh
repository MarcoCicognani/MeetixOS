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

#include <TC/Traits/IsSame.hh>
#include <TC/Traits/RemoveConstVolatile.hh>

namespace TC::Traits {

template<typename T>
struct IsVoid : IsSame<void, typename RemoveConstVolatile<T>::Type> {
    /* Empty Body */
};

} /* namespace TC::Traits */