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

#include <Api.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#if defined(__cplusplus) && defined(__GNUC__)
#    define __assert_function __PRETTY_FUNCTION__
#elif __STDC_VERSION__ >= 199901L
#    define __assert_function __func__
#else
#    define __assert_function ((char*)0)
#endif

#undef assert

/* disable assert for NDEBUG disabled */
#ifdef NDEBUG
#    define assert(ignore) ((void)0)
#endif

#ifndef NDEBUG
#    define assert(expr) ((expr) ? (void)0 : assert_failed(__FILE__, __LINE__, __assert_function, #    expr))
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#    undef static_assert
#    define static_assert _Static_assert
#endif

/* ------------------------------------ C function prototypes ----------------------------------- */

void assert_failed(const char*, int, const char*, const char*);

#ifdef __cplusplus
}
#endif
