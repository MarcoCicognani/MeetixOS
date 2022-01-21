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

#include <TC/Traits/AddConst.hh>
#include <TC/Traits/Conditional.hh>
#include <TC/Traits/IsConst.hh>
#include <TC/Traits/RemoveConst.hh>

namespace TC::Traits {

template<typename ReferenceType, typename T>
using CopyConst = Conditional<IsConst<ReferenceType>, AddConst<T>, RemoveConst<T>>;

} /* namespace TC::Traits */