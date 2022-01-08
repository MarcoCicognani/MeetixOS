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

#include <TC/Traits/FalseType.hh>
#include <TC/Traits/RemoveConstVolatile.hh>
#include <TC/Traits/TrueType.hh>

namespace TC::Traits {
namespace Details {

template<typename T>
struct IsPointer : FalseType {
    /* Empty Body */
};

template<typename T>
struct IsPointer<T*> : TrueType {
    /* Empty Body */
};

} /* namespace Details */

template<typename T>
struct IsPointer : Details::IsPointer<typename RemoveConstVolatile<T>::Type> {
    /* Empty Body */
};

} /* namespace TC::Traits */