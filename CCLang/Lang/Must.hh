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

#include <CCLang/Core/Assertions.hh>

#define must$(expression)                                                                                                   \
    ({                                                                                                                      \
        auto __tryable = (expression);                                                                                      \
        verify_false_with_msg$(__tryable.__is_bad_variant(), "must$(`" #expression "`) has produced a bad variant result"); \
        __tryable.unwrap();                                                                                                 \
    })
