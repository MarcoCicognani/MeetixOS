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

/**
 * @brief Works like Rust's try!() macro: If <expression> produces an error Result returns the error to the caller
 * function, otherwise unwraps the value to the current function
 */
#define TRY(expression)                                                                                                \
    ({                                                                                                                 \
        auto expression_result = (expression);                                                                         \
        if ( expression_result.is_error() )                                                                            \
            return expression_result.unwrap_error();                                                                   \
        expression_result.unwrap_value();                                                                              \
    })
