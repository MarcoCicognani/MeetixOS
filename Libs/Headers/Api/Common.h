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
 * C++ compilers must know that the declarations are C declarations
 */
#ifdef __cplusplus
#    define __BEGIN_C extern "C" {
#    define __END_C   }
#else
#    define __BEGIN_C
#    define __END_C
#endif

/**
 * @brief CPP check for STD_C minimum version
 */
#define __HAS_STDC11 (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)

/**
 * @brief Shows the message on the s_log for a non implemented function
 */
#define __NOT_IMPLEMENTED(name) s_log("Warning: Function '" #name "' is not implemented")

/**
 * @brief Debug tracing
 */
#define __DEBUG_TRACE_ENABLED 0
#if __DEBUG_TRACE_ENABLED
#    define __DEBUG_TRACE(function) s_log(#    function);
#else
#    define __DEBUG_TRACE(function)
#endif

/**
 * @brief Compiler attributes
 */
#define A_PACKED                       __attribute__((packed))
#define A_WEAK                         __attribute__((weak))
#define A_UNUSED                       __attribute__((unused))
#define A_NOINLINE                     __attribute__((noinline))
#define A_NORETURN                     __attribute__((noreturn))
#define A_PRINTF(fmt_pos, va_list_pos) __attribute__((format(printf, fmt_pos, va_list_pos)))