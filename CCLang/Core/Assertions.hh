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

#define __verify_internal$(expression, failure_message)                                                                                                        \
    do {                                                                                                                                                       \
        if ( !(expression) ) [[unlikely]] {                                                                                                                    \
            panic(failure_message, __builtin_FILE(), __builtin_LINE());                                                                                        \
        }                                                                                                                                                      \
    } while ( false )

#define verify_equal_with_msg$(lhs, rhs, msg)         __verify_internal$(lhs == rhs, msg)
#define verify_not_equal_with_msg$(lhs, rhs, msg)     __verify_internal$(lhs != rhs, msg)
#define verify_greater_with_msg$(lhs, rhs, msg)       __verify_internal$(lhs > rhs, msg)
#define verify_greater_equal_with_msg$(lhs, rhs, msg) __verify_internal$(lhs >= rhs, msg)
#define verify_less_with_msg$(lhs, rhs, msg)          __verify_internal$(lhs < rhs, msg)
#define verify_less_equal_with_msg$(lhs, rhs, msg)    __verify_internal$(lhs <= rhs, msg)
#define verify_not_reached$()                         __verify_internal$(false, "Reached an unreachable instruction")

#define verify_with_msg$(expr, msg)          verify_equal_with_msg$(expr, true, msg)
#define verify_false_with_msg$(expr, msg)    verify_equal_with_msg$(expr, false, msg)
#define verify_null_with_msg$(expr, msg)     verify_equal_with_msg$(expr, nullptr, msg)
#define verify_not_null_with_msg$(expr, msg) verify_not_equal_with_msg$(expr, nullptr, msg)

#define verify_equal$(lhs, rhs)         verify_equal_with_msg$(lhs, rhs, "`==` - `" #lhs "` is not equal to `" #rhs "`")
#define verify_not_equal$(lhs, rhs)     verify_not_equal_with_msg$(lhs, rhs, "`!=` - `" #lhs "` is equal to `" #rhs "`")
#define verify_greater$(lhs, rhs)       verify_greater_with_msg$(lhs, rhs, "`>`- `" #lhs "` is not greater than `" #rhs "`")
#define verify_greater_equal$(lhs, rhs) verify_greater_equal_with_msg$(lhs, rhs, "`>=` - `" #lhs "` is not greater or equal than `" #rhs "`")
#define verify_less$(lhs, rhs)          verify_less_with_msg$(lhs, rhs, "`<` - `" #lhs "` is not less than `" #rhs "`")
#define verify_less_equal$(lhs, rhs)    verify_less_equal_with_msg$(lhs, rhs, "`<=` - `" #lhs "` is not greater or equal than `" #rhs "`")

#define verify$(expr)          verify_equal$(expr, true)
#define verify_false$(expr)    verify_equal$(expr, false)
#define verify_null$(expr)     verify_equal$(expr, nullptr)
#define verify_not_null$(expr) verify_not_equal$(expr, nullptr)

[[nodiscard]]
auto panic(char const* msg, char const* file = __builtin_FILE(), int line = __builtin_LINE()) -> void;

namespace Details {

[[noreturn]]
auto __assert_plug_panic(char const* expression, char const* file, int line) -> void;

} /* namespace Details */
