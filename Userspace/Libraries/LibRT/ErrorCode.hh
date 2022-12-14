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

enum class ErrorCode : unsigned short int {
    None = 0,
    BadData,
    BadFile,
    BadLength,
    BadParameter,
    Denied,
    DivisionByZero,
    EmptyData,
    Existing,
    IndexOutOfRange,
    IntOverflow,
    Invalid,
    NoMemory,
    NotFound,
    NullPointer,
    ShiftOverflow,
    Unimplemented,
};
