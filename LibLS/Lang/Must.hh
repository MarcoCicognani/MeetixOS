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

#include <LibLS/Core/Assertions.hh>

#define must$(expression)                     \
    ({                                        \
        auto __tryable_result = (expression);                                                                                                                  \
        verify$(__tryable_result.is_value());  \
        __tryable_result.unwrap_value();      \
    })
