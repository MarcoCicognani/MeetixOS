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

#include <Api/Common.h>

#ifndef __cplusplus
#    define static_assert _Static_assert
#endif

#define __stringify_helper(x) #x
#define __stringify(x)        __stringify_helper(x)

#undef assert

#ifndef NDEBUG
#    define assert(expression)                                                                                         \
        (__builtin_expect(!(expression), 0) ? assert_failed(#expression "\n" __FILE__ ":" __stringify(__LINE__))       \
                                            : (void)0)
#else
#    define assert(expression) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

A_NORETURN void assert_failed(const char*);

#ifdef __cplusplus
}
#endif
