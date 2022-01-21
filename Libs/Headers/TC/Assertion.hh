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

#include <TC/IntTypes.hh>

namespace TC::Assertion {

/**
 * @brief Implemented into LibC/assert/assert_failed.cc for userspace and ??? for Kernel
 */
[[noreturn]] void assertion_failed(const char* message, const char* file, u32 line, const char* function);

} /* namespace TC::Assertion */

#define VERIFY(expression)                                                                                             \
    do {                                                                                                               \
        if ( !static_cast<bool>(expression) ) [[unlikely]]                                                             \
            TC::Assertion::assertion_failed(#expression, __FILE__, __LINE__, __PRETTY_FUNCTION__);                     \
    } while ( false )

#define VERIFY_NOT_REACHED() VERIFY(false)
