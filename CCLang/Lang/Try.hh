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

#define try$(expression)                                   \
    ({                                                     \
        auto __tryable = (expression);                     \
        if ( __tryable.__is_bad_variant() ) [[unlikely]] { \
            return __tryable.__propagate_failure();        \
        }                                                  \
        __tryable.unwrap();                                \
    })
