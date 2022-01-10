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
constexpr typename TC::Traits::RemoveReference<T>::Type&& move(T&& arg) noexcept {
    return static_cast<typename TC::Traits::RemoveReference<T>::Type&&>(arg);
}

} /* namespace std */