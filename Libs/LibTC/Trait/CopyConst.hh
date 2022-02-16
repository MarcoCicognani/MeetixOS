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

#include <LibTC/Trait/AddConst.hh>
#include <LibTC/Trait/Conditional.hh>
#include <LibTC/Trait/IsConst.hh>
#include <LibTC/Trait/RemoveConst.hh>

namespace TC {
namespace Trait {

template<typename ReferenceType, typename T>
using CopyConst = Conditional<IsConst<ReferenceType>, AddConst<T>, RemoveConst<T>>;

} /* namespace Trait */

using Trait::CopyConst;

} /* namespace TC */