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
#pragma clang diagnostic push
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"

#include <Api/Common.h>

#ifndef __cplusplus
#    define static_assert _Static_assert
#endif

#define __stringify_helper(x) #x
#define __stringify(x)        __stringify_helper(x)

#undef assert

#ifndef NDEBUG
#    define assert(expression) (__builtin_expect(!(expression), 0) ? libc_runtime_panic(__FILE__, __LINE__, #    expression) : (void)0)
#else
#    define assert(expression) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

A_NORETURN void libc_runtime_panic(const char* file, int line, const char* expression);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
