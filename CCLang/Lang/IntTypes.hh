/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <CCLang/Forward.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Details.hh>
#include <CCLang/Lang/Must.hh>

OOUnsignedIntegerWrapper$(usize, __SIZE_TYPE__, __SIZE_MAX__); /* First because is used by bit_count() */
template<typename T>
auto operator+(T* ptr, usize offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, usize offset) -> T* {
    return ptr - offset.unwrap();
}

OOUnsignedIntegerWrapper$(u8, __UINT8_TYPE__, __UINT8_MAX__);
OOUnsignedIntegerWrapper$(u16, __UINT16_TYPE__, __UINT16_MAX__);
OOUnsignedIntegerWrapper$(u32, __UINT32_TYPE__, __UINT32_MAX__);
OOUnsignedIntegerWrapper$(u64, __UINT32_TYPE__, __UINT64_MAX__);

OOSignedIntegerWrapper$(i8, __INT8_TYPE__, __INT8_MAX__, -__INT8_MAX__ - 1);
OOSignedIntegerWrapper$(i16, __INT16_TYPE__, __INT16_MAX__, -__INT16_MAX__ - 1);
OOSignedIntegerWrapper$(i32, __INT32_TYPE__, __INT32_MAX__, -__INT32_MAX__ - 1);
OOSignedIntegerWrapper$(i64, __INT64_TYPE__, __INT64_MAX__, -__INT64_MAX__ - 1);
OOSignedIntegerWrapper$(isize, __PTRDIFF_TYPE__, __PTRDIFF_MAX__, -__PTRDIFF_MAX__ - 1);

#ifndef IN_KERNEL
using f32 = float;
using f64 = double;
using f80 = long double;
#endif
