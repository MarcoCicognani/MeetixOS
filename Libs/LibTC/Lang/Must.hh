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

#include <LibTC/Core/Assertions.hh>

#define must$(expression)                                                                                                                                      \
    ({                                                                                                                                                         \
        auto expression_result = (expression);                                                                                                                 \
        VERIFY(expression_result.is_value());                                                                                                                  \
        expression_result.unwrap_value();                                                                                                                      \
    })
