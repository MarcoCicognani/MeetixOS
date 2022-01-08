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
struct MakeSigned {
    /* Empty Body */
};

template<>
struct MakeSigned<u8> {
    using Type = i8;
};

template<>
struct MakeSigned<u16> {
    using Type = i16;
};

template<>
struct MakeSigned<u32> {
    using Type = i32;
};

template<>
struct MakeSigned<u64> {
    using Type = i64;
};

template<>
struct MakeSigned<i8> {
    using Type = i8;
};

template<>
struct MakeSigned<i16> {
    using Type = i16;
};

template<>
struct MakeSigned<i32> {
    using Type = i32;
};

template<>
struct MakeSigned<i64> {
    using Type = i64;
};

} /* namespace TC::Traits */