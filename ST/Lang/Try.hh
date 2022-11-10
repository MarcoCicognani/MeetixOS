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

/**
 * @brief Works like Rust's try!() macro: If <expression> produces an empty or error variant then it returns the code to the
 * caller function, otherwise unwraps the value to the current function
 */
#define try$(expression)                         \
    ({                                           \
        auto __tryable_result = (expression);    \
        if ( !__tryable_result ) [[unlikely]] {  \
            return __tryable_result.propagate(); \
        }                                        \
        __tryable_result.unwrap();               \
    })
