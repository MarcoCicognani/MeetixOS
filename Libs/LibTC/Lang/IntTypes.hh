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

using u8    = __UINT8_TYPE__;
using u16   = __UINT16_TYPE__;
using u32   = __UINT32_TYPE__;
using u64   = __UINT64_TYPE__;
using usize = __SIZE_TYPE__;

using i8    = __INT8_TYPE__;
using i16   = __INT16_TYPE__;
using i32   = __INT32_TYPE__;
using i64   = __INT64_TYPE__;
using isize = __PTRDIFF_TYPE__;

#ifndef IN_KERNEL
using f32 = float;
using f64 = double;
using f80 = long double;
#endif
