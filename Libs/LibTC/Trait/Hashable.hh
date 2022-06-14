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

#include <LibTC/IntTypes.hh>
#include <LibTC/Trait/IsSame.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Trait {

template<typename T>
concept Hashable = requires(T value) {
                       { TypeIntrinsics<T>::hash(value) } -> SameAs<usize>;
                   };

} /* namespace Trait */

using Trait::Hashable;

} /* namespace TC */
