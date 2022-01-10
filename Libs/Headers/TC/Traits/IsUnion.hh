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

#include <TC/Traits/Constant.hh>

namespace TC::Traits {

template<typename T>
struct IsUnion : public Constant<bool, __is_union(T)> {
    /* Empty Body */
};

} /* namespace TC::Traits */