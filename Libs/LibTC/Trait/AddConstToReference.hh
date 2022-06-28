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

namespace TC {
namespace Trait {
namespace Details {

template<class T>
struct AddConstToReference {
    using Type = T;
};

template<class T>
struct AddConstToReference<T&> {
    using Type = AddConst<T>&;
};

template<class T>
struct AddConstToReference<T&&> {
    using Type = AddConst<T>&&;
};

} /* namespace Details */

template<class T>
using AddConstToReference = typename Details::AddConstToReference<T>::Type;

} /* namespace Trait */

using Trait::AddConstToReference;

} /* namespace TC */