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
struct MakeUnsigned {
    /* Empty Body */
};

template<>
struct MakeUnsigned<signed char> {
    using Type = unsigned char;
};

template<>
struct MakeUnsigned<signed short> {
    using Type = unsigned short;
};

template<>
struct MakeUnsigned<signed int> {
    using Type = unsigned int;
};

template<>
struct MakeUnsigned<signed long> {
    using Type = unsigned long;
};

template<>
struct MakeUnsigned<signed long long> {
    using Type = unsigned long long;
};

template<>
struct MakeUnsigned<unsigned char> {
    using Type = unsigned char;
};

template<>
struct MakeUnsigned<unsigned short> {
    using Type = unsigned short;
};

template<>
struct MakeUnsigned<unsigned int> {
    using Type = unsigned int;
};

template<>
struct MakeUnsigned<unsigned long> {
    using Type = unsigned long;
};

template<>
struct MakeUnsigned<unsigned long long> {
    using Type = unsigned long long;
};

} /* namespace Details */

template<typename T>
using MakeUnsigned = typename Details::MakeUnsigned<T>::Type;

} /* namespace TC::Trait */