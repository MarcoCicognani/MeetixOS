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

namespace TC::Trait {
namespace Details {

template<typename T>
struct RemovePointer {
    using Type = T;
};

template<typename T>
struct RemovePointer<T*> {
    using Type = T;
};

template<typename T>
struct RemovePointer<T* const> {
    using Type = T;
};

template<typename T>
struct RemovePointer<T* volatile> {
    using Type = T;
};

template<typename T>
struct RemovePointer<T* const volatile> {
    using Type = T;
};

} /* namespace Details */

template<typename T>
using RemovePointer = typename Details::RemovePointer<T>::Type;

} /* namespace TC::Trait */