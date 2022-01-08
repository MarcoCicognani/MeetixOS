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

namespace TC::Traits {

template<typename T>
struct MakeUnsigned {
    /* Empty Body */
};

template<>
struct MakeUnsigned<i8> {
    using Type = u8;
};

template<>
struct MakeUnsigned<i16> {
    using Type = u16;
};

template<>
struct MakeUnsigned<i32> {
    using Type = u32;
};

template<>
struct MakeUnsigned<i64> {
    using Type = u64;
};

template<>
struct MakeUnsigned<u8> {
    using Type = u8;
};

template<>
struct MakeUnsigned<u16> {
    using Type = u16;
};

template<>
struct MakeUnsigned<u32> {
    using Type = u32;
};

template<>
struct MakeUnsigned<u64> {
    using Type = u64;
};

} /* namespace TC::Traits */