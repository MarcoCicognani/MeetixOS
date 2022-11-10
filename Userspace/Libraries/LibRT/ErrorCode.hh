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

#include <ST/Lang/IntTypes.hh>

enum class ErrorCode : u16 {
    Denied = 1,
    Existing,
    Invalid,
    NoMemory,
    BadFile,
    NotFound,
    IntOverflow,
    EmptyData,
    BadLength,
    BadData
};
