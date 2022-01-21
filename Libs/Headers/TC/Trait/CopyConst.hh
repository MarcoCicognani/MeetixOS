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

#include <TC/Trait/AddConst.hh>
#include <TC/Trait/Conditional.hh>
#include <TC/Trait/IsConst.hh>
#include <TC/Trait/RemoveConst.hh>

namespace TC::Trait {

template<typename ReferenceType, typename T>
using CopyConst = Conditional<IsConst<ReferenceType>, AddConst<T>, RemoveConst<T>>;

} /* namespace TC::Trait */