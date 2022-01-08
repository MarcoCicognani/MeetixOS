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
#include <TC/Traits/TrueType.hh>

namespace TC::Traits {

template<typename T>
struct IsLValue : FalseType {
    /* Empty Body */
};

template<typename T>
struct IsLValue<T&> : TrueType {
    /* Empty Body */
};

} /* namespace TC::Traits */