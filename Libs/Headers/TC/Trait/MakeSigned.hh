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

namespace TC {
namespace Trait {
namespace Details {

template<typename T>
struct MakeSigned {
    using Type = void;
};

template<>
struct MakeSigned<unsigned char> {
    using Type = signed char;
};

template<>
struct MakeSigned<unsigned short> {
    using Type = signed short;
};

template<>
struct MakeSigned<unsigned int> {
    using Type = signed int;
};

template<>
struct MakeSigned<unsigned long> {
    using Type = signed long;
};

template<>
struct MakeSigned<unsigned long long> {
    using Type = signed long long;
};

template<>
struct MakeSigned<signed char> {
    using Type = signed char;
};

template<>
struct MakeSigned<signed short> {
    using Type = signed short;
};

template<>
struct MakeSigned<signed int> {
    using Type = signed int;
};

template<>
struct MakeSigned<signed long> {
    using Type = signed long;
};

template<>
struct MakeSigned<signed long long> {
    using Type = signed long long;
};

} /* namespace Details */

template<typename T>
using MakeSigned = typename Details::MakeSigned<T>::Type;

} /* namespace Trait */

using Trait::MakeSigned;

} /* namespace TC */