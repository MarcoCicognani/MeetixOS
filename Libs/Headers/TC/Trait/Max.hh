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

#include <TC/IntTypes.hh>

namespace TC::Trait {

template<usize a, usize... others>
struct Max {
    /* Empty Body */
};

template<usize arg>
struct Max<arg> {
    static constexpr usize value = arg;
};

template<usize a, usize b, usize... others>
struct Max<a, b, others...> {
    static constexpr usize value = a >= b ? Max<a, others...>::value : Max<b, others...>::value;
};

} /* namespace TC::Trait */