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

#include <Api/Common.h>
#include <stddef.h>

__BEGIN_C

/**
 * @brief Exact-width integer types
 */
typedef __UINT8_TYPE__  uint8_t;
typedef __INT8_TYPE__   int8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __INT16_TYPE__  int16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __INT32_TYPE__  int32_t;
typedef __UINT64_TYPE__ uint64_t;
typedef __INT64_TYPE__  int64_t;

/**
 * @brief Minimum-width integer types
 */
typedef __INT_LEAST8_TYPE__   int_least8_t;
typedef __UINT_LEAST8_TYPE__  uint_least8_t;
typedef __INT_LEAST16_TYPE__  int_least16_t;
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
typedef __INT_LEAST32_TYPE__  int_least32_t;
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
typedef __INT_LEAST64_TYPE__  int_least64_t;
typedef __UINT_LEAST64_TYPE__ uint_least64_t;

/**
 * @brief Fastest minimum-width integer types
 */
typedef __INT_FAST8_TYPE__   int_fast8_t;
typedef __UINT_FAST8_TYPE__  uint_fast8_t;
typedef __INT_FAST16_TYPE__  int_fast16_t;
typedef __UINT_FAST16_TYPE__ uint_fast16_t;
typedef __INT_FAST32_TYPE__  int_fast32_t;
typedef __UINT_FAST32_TYPE__ uint_fast32_t;
typedef __INT_FAST64_TYPE__  int_fast64_t;
typedef __UINT_FAST64_TYPE__ uint_fast64_t;

/**
 * @brief Integer type capable of holding the value of a pointer
 */
typedef __INTPTR_TYPE__  intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;

/**
 * @brief Greatest-width integer types
 */
typedef __INTMAX_TYPE__  intmax_t;
typedef __UINTMAX_TYPE__ uintmax_t;

/**
 * @brief Limits of exact-width integer types
 */
#define INT8_MAX   (__INT8_MAX__)
#define INT8_MIN   (-__INT8_MAX__ - 1)
#define INT16_MAX  (__INT16_MAX__)
#define INT16_MIN  (-__INT16_MAX__ - 1)
#define INT32_MAX  (__INT32_MAX__)
#define INT32_MIN  (-__INT32_MAX__ - 1)
#define INT64_MAX  (__INT64_MAX__)
#define INT64_MIN  (-__INT64_MAX__ - 1)
#define UINT8_MAX  (__UINT8_MAX__)
#define UINT16_MAX (__UINT16_MAX__)
#define UINT32_MAX (__UINT32_MAX__)
#define UINT64_MAX (__UINT64_MAX__)

/**
 * @brief Limits of minimum-width integer types
 */
#define INT_LEAST8_MAX   (__INT_LEAST8_MAX__)
#define INT_LEAST8_MIN   (-__INT_LEAST8_MAX__ - 1)
#define INT_LEAST16_MAX  (__INT_LEAST16_MAX__)
#define INT_LEAST16_MIN  (-__INT_LEAST16_MAX__ - 1)
#define INT_LEAST32_MAX  (__INT_LEAST32_MAX__)
#define INT_LEAST32_MIN  (-__INT_LEAST32_MAX__ - 1)
#define INT_LEAST64_MAX  (__INT_LEAST64_MAX__)
#define INT_LEAST64_MIN  (-__INT_LEAST64_MAX__ - 1)
#define UINT_LEAST8_MAX  (__UINT_LEAST8_MAX__)
#define UINT_LEAST16_MAX (__UINT_LEAST16_MAX__)
#define UINT_LEAST32_MAX (__UINT_LEAST32_MAX__)
#define UINT_LEAST64_MAX (__UINT_LEAST64_MAX__)

/**
 * @brief Limits of fastest minimum-width integer types
 */
#define INT_FAST8_MAX   (__INT_FAST8_MAX__)
#define INT_FAST8_MIN   (-__INT_FAST8_MAX__ - 1)
#define INT_FAST16_MAX  (__INT_FAST16_MAX__)
#define INT_FAST16_MIN  (-__INT_FAST16_MAX__ - 1)
#define INT_FAST32_MAX  (__INT_FAST32_MAX__)
#define INT_FAST32_MIN  (-__INT_FAST32_MAX__ - 1)
#define INT_FAST64_MAX  (__INT_FAST64_MAX__)
#define INT_FAST64_MIN  (-__INT_FAST64_MAX__ - 1)
#define UINT_FAST8_MAX  (__UINT_FAST8_MAX__)
#define UINT_FAST16_MAX (__UINT_FAST16_MAX__)
#define UINT_FAST32_MAX (__UINT_FAST32_MAX__)
#define UINT_FAST64_MAX (__UINT_FAST64_MAX__)

/**
 * @brief Limits of integer types capable of holding object pointers
 */
#define INTPTR_MAX  (__INTPTR_MAX__)
#define INTPTR_MIN  (-__INTPTR_MAX__ - 1)
#define UINTPTR_MAX (__UINTPTR_MAX__)

/**
 * @brief Limits of greatest-width integer types
 */
#define INTMAX_MAX  (__INTMAX_MAX__)
#define INTMAX_MIN  (-__INTMAX_MAX__ - 1)
#define UINTMAX_MAX (__UINTMAX_MAX__)

/**
 * @brief Limits of other integer types
 */
#define PTRDIFF_MAX (__PTRDIFF_MAX__)
#define PTRDIFF_MIN (-__PTRDIFF_MAX__ - 1)

#define SIG_ATOMIC_MAX (__SIG_ATOMIC_MAX__)
#define SIG_ATOMIC_MIN (__SIG_ATOMIC_MIN__)

#define SIZE_MAX (__SIZE_MAX__)

#define WCHAR_MAX (__WCHAR_MAX__)
#define WCHAR_MIN (__WCHAR_MIN__)

#define WINT_MAX (__WINT_MAX__)
#define WINT_MIN (__WINT_MIN__)

/**
 * @brief Macros for minimum-width integer constants
 */
#define INT8_C(c)  (__INT8_C(c))
#define INT16_C(c) (__INT16_C(c))
#define INT32_C(c) (__INT32_C(c))
#define INT64_C(c) (__INT64_C(c))

#define UINT8_C(c)  (__UINT8_C(c))
#define UINT16_C(c) (__UINT16_C(c))
#define UINT32_C(c) (__UINT32_C(c))
#define UINT64_C(c) (__UINT64_C(c))

/**
 * @brief Macros for greatest-width integer constants
 */
#define INTMAX_C(c)  (__INTMAX_C(c))
#define UINTMAX_C(c) (__UINTMAX_C(c))

/**
 * @brief MeetiX specific unsigned integer types
 */
typedef uint8_t      u8;
typedef uint16_t     u16;
typedef uint32_t     u32;
typedef uint64_t     u64;
typedef unsigned int usize;

/**
 * @brief MeetiX specific signed integer types
 */
typedef int8_t     i8;
typedef int16_t    i16;
typedef int32_t    i32;
typedef int64_t    i64;
typedef signed int isize;

__END_C
