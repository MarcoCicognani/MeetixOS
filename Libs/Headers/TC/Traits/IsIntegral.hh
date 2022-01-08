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

#include <Api/StdInt.h>
#include <TC/Traits/FalseType.hh>
#include <TC/Traits/MakeUnsigned.hh>
#include <TC/Traits/RemoveConstVolatile.hh>
#include <TC/Traits/TrueType.hh>

namespace TC::Traits {
namespace Details {

template<typename T>
struct IsIntegral : FalseType {
    /* Empty Body */
};

template<>
struct IsIntegral<u8> : TrueType {
    /* Empty Body */
};

template<>
struct IsIntegral<u16> : TrueType {
    /* Empty Body */
};

template<>
struct IsIntegral<u32> : TrueType {
    /* Empty Body */
};

template<>
struct IsIntegral<u64> : TrueType {
    /* Empty Body */
};

} /* namespace Details */

template<typename T>
using IsIntegral = Details::IsIntegral<typename MakeUnsigned<typename RemoveConstVolatile<T>::Type>::Type>;

} /* namespace TC::Traits */