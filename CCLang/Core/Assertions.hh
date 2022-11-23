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

#define __verify_internal$(expression)                                                     \
    do {                                                                                   \
        if ( __builtin_expect(!static_cast<bool>(expression), 0) )                         \
            Details::__assert_plug_panic(#expression,  __builtin_FILE(), __builtin_LINE()); \
    } while ( false )

#define verify$(expression)                  __verify_internal$(expression)
#define verify_equal$(first, second)         __verify_internal$(first == second)
#define verify_false$(expression)            __verify_internal$(!expression)
#define verify_greater$(first, second)       __verify_internal$(first > second)
#define verify_greater_equal$(first, second) __verify_internal$(first >= second)
#define verify_less$(first, second)          __verify_internal$(first < second)
#define verify_less_equal$(first, second)    __verify_internal$(first <= second)
#define verify_not_equal$(first, second)     __verify_internal$(first != second)
#define verify_not_null$(expression)         __verify_internal$(expression != nullptr)
#define verify_null$(expression)             __verify_internal$(expression == nullptr)
#define verify_not_reached$()                __verify_internal$(false)

namespace Details {

[[noreturn]]
auto __assert_plug_panic(char const* expression, char const* file, int line) -> void;

} /* namespace Details */
