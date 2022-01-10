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

#include <TC/Traits/RemoveReference.hh>

namespace std {

template<typename T>
constexpr T&& forward(typename TC::Traits::RemoveReference<T>::Type& param) noexcept {
    return static_cast<T&&>(param);
}

template<typename T>
constexpr T&& forward(typename TC::Traits::RemoveReference<T>::Type&& param) noexcept {
    return static_cast<T&&>(param);
}

} /* namespace std */