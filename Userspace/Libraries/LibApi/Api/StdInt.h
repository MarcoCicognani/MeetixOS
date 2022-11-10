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
#pragma ide diagnostic   ignored "bugprone-macro-parentheses"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <Api/Common.h>

#ifdef __cplusplus
#    define TYPE_ALIAS(TAlias, TReal) using TAlias = TReal
#else
#    define TYPE_ALIAS(TAlias, TReal) typedef TReal TAlias
#endif

#ifdef __cplusplus
#    define CONST_VALUE(Name, T, Value) static constexpr T Name = Value
#else
#    define CONST_VALUE(Name, T, Value) static const T Name = Value
#endif

TYPE_ALIAS(int8_t, __INT8_TYPE__);
TYPE_ALIAS(uint8_t, __UINT8_TYPE__);

TYPE_ALIAS(int16_t, __INT16_TYPE__);
TYPE_ALIAS(uint16_t, __UINT16_TYPE__);

TYPE_ALIAS(int32_t, __INT32_TYPE__);
TYPE_ALIAS(uint32_t, __UINT32_TYPE__);

TYPE_ALIAS(int64_t, __INT64_TYPE__);
TYPE_ALIAS(uint64_t, __UINT64_TYPE__);

TYPE_ALIAS(int_least8_t, __INT_LEAST8_TYPE__);
TYPE_ALIAS(uint_least8_t, __UINT_LEAST8_TYPE__);

TYPE_ALIAS(int_least16_t, __INT_LEAST16_TYPE__);
TYPE_ALIAS(uint_least16_t, __UINT_LEAST16_TYPE__);

TYPE_ALIAS(int_least32_t, __INT_LEAST32_TYPE__);
TYPE_ALIAS(uint_least32_t, __UINT_LEAST32_TYPE__);

TYPE_ALIAS(int_least64_t, __INT_LEAST64_TYPE__);
TYPE_ALIAS(uint_least64_t, __UINT_LEAST64_TYPE__);

TYPE_ALIAS(int_fast8_t, __INT_FAST8_TYPE__);
TYPE_ALIAS(uint_fast8_t, __UINT_FAST8_TYPE__);

TYPE_ALIAS(int_fast16_t, __INT_FAST16_TYPE__);
TYPE_ALIAS(uint_fast16_t, __UINT_FAST16_TYPE__);

TYPE_ALIAS(int_fast32_t, __INT_FAST32_TYPE__);
TYPE_ALIAS(uint_fast32_t, __UINT_FAST32_TYPE__);

TYPE_ALIAS(int_fast64_t, __INT_FAST64_TYPE__);
TYPE_ALIAS(uint_fast64_t, __UINT_FAST64_TYPE__);

TYPE_ALIAS(intptr_t, __INTPTR_TYPE__);
TYPE_ALIAS(uintptr_t, __UINTPTR_TYPE__);
TYPE_ALIAS(ptrdiff_t, __PTRDIFF_TYPE__);

TYPE_ALIAS(intmax_t, __INTMAX_TYPE__);
TYPE_ALIAS(uintmax_t, __UINTMAX_TYPE__);

TYPE_ALIAS(size_t, __SIZE_TYPE__);

/* a little trick to ensure that __SIZE_TYPE__ is signed */
#define unsigned signed
TYPE_ALIAS(ssize_t, __SIZE_TYPE__);
#undef unsigned

/* MeetixOS custom types */
/* TODO Unificate with STC/IntTypes.hh */

TYPE_ALIAS(i8, int8_t);
TYPE_ALIAS(u8, uint8_t);

TYPE_ALIAS(i16, int16_t);
TYPE_ALIAS(u16, uint16_t);

TYPE_ALIAS(i32, int32_t);
TYPE_ALIAS(u32, uint32_t);

TYPE_ALIAS(i64, int64_t);
TYPE_ALIAS(u64, uint64_t);

TYPE_ALIAS(isize, ssize_t);
TYPE_ALIAS(usize, size_t);

CONST_VALUE(INT8_MAX, int8_t, __INT8_MAX__);
CONST_VALUE(INT8_MIN, int8_t, -__INT8_MAX__ - 1);

CONST_VALUE(INT16_MAX, int16_t, __INT16_MAX__);
CONST_VALUE(INT16_MIN, int16_t, -__INT16_MAX__ - 1);

CONST_VALUE(INT32_MAX, int32_t, __INT32_MAX__);
CONST_VALUE(INT32_MIN, int32_t, -__INT32_MAX__ - 1);

CONST_VALUE(INT64_MAX, int64_t, __INT64_MAX__);
CONST_VALUE(INT64_MIN, int64_t, -__INT64_MAX__ - 1);

CONST_VALUE(UINT8_MAX, uint8_t, __UINT8_MAX__);
CONST_VALUE(UINT16_MAX, uint16_t, __UINT16_MAX__);
CONST_VALUE(UINT32_MAX, uint32_t, __UINT32_MAX__);
CONST_VALUE(UINT64_MAX, uint64_t, __UINT64_MAX__);

CONST_VALUE(INT_LEAST8_MAX, int_least8_t, __INT_LEAST8_MAX__);
CONST_VALUE(INT_LEAST8_MIN, int_least8_t, -__INT_LEAST8_MAX__ - 1);

CONST_VALUE(INT_LEAST16_MAX, int_least16_t, __INT_LEAST16_MAX__);
CONST_VALUE(INT_LEAST16_MIN, int_least16_t, -__INT_LEAST16_MAX__ - 1);

CONST_VALUE(INT_LEAST32_MAX, int_least32_t, __INT_LEAST32_MAX__);
CONST_VALUE(INT_LEAST32_MIN, int_least32_t, -__INT_LEAST32_MAX__ - 1);

CONST_VALUE(INT_LEAST64_MAX, int_least64_t, __INT_LEAST64_MAX__);
CONST_VALUE(INT_LEAST64_MIN, int_least64_t, -__INT_LEAST64_MAX__ - 1);

CONST_VALUE(UINT_LEAST8_MAX, uint_least8_t, __UINT_LEAST8_MAX__);
CONST_VALUE(UINT_LEAST16_MAX, uint_least16_t, __UINT_LEAST16_MAX__);
CONST_VALUE(UINT_LEAST32_MAX, uint_least32_t, __UINT_LEAST32_MAX__);
CONST_VALUE(UINT_LEAST64_MAX, uint_least64_t, __UINT_LEAST64_MAX__);

CONST_VALUE(INT_FAST8_MAX, int_fast8_t, __INT_FAST8_MAX__);
CONST_VALUE(INT_FAST8_MIN, int_fast8_t, -__INT_FAST8_MAX__ - 1);

CONST_VALUE(INT_FAST16_MAX, int_fast16_t, __INT_FAST16_MAX__);
CONST_VALUE(INT_FAST16_MIN, int_fast16_t, -__INT_FAST16_MAX__ - 1);

CONST_VALUE(INT_FAST32_MAX, int_fast32_t, __INT_FAST32_MAX__);
CONST_VALUE(INT_FAST32_MIN, int_fast32_t, -__INT_FAST32_MAX__ - 1);

CONST_VALUE(INT_FAST64_MAX, int_fast64_t, __INT_FAST64_MAX__);
CONST_VALUE(INT_FAST64_MIN, int_fast64_t, -__INT_FAST64_MAX__ - 1);

CONST_VALUE(UINT_FAST8_MAX, uint_fast8_t, __UINT_FAST8_MAX__);
CONST_VALUE(UINT_FAST16_MAX, uint_fast16_t, __UINT_FAST16_MAX__);
CONST_VALUE(UINT_FAST32_MAX, uint_fast32_t, __UINT_FAST32_MAX__);
CONST_VALUE(UINT_FAST64_MAX, uint_fast64_t, __UINT_FAST64_MAX__);

CONST_VALUE(INTPTR_MAX, intptr_t, __INTPTR_MAX__);
CONST_VALUE(INTPTR_MIN, intptr_t, -__INTPTR_MAX__ - 1);
CONST_VALUE(UINTPTR_MAX, uintptr_t, __UINTPTR_MAX__);

CONST_VALUE(INTMAX_MAX, intmax_t, __INTMAX_MAX__);
CONST_VALUE(INTMAX_MIN, intmax_t, -__INTMAX_MAX__ - 1);
CONST_VALUE(UINTMAX_MAX, uintmax_t, __UINTMAX_MAX__);

CONST_VALUE(PTRDIFF_MAX, ptrdiff_t, __PTRDIFF_MAX__);
CONST_VALUE(PTRDIFF_MIN, ptrdiff_t, -__PTRDIFF_MAX__ - 1);

CONST_VALUE(SIG_ATOMIC_MAX, int32_t, __SIG_ATOMIC_MAX__);
CONST_VALUE(SIG_ATOMIC_MIN, int32_t, __SIG_ATOMIC_MIN__);

CONST_VALUE(SIZE_MAX, size_t, __SIZE_MAX__);

CONST_VALUE(WCHAR_MAX, uint32_t, __WCHAR_MAX__);
CONST_VALUE(WCHAR_MIN, uint32_t, __WCHAR_MIN__);

CONST_VALUE(WINT_MAX, uint32_t, __WINT_MAX__);
CONST_VALUE(WINT_MIN, uint32_t, __WINT_MIN__);

#define INT8_C(c)  (__INT8_C(c))
#define INT16_C(c) (__INT16_C(c))
#define INT32_C(c) (__INT32_C(c))
#define INT64_C(c) (__INT64_C(c))

#define UINT8_C(c)  (__UINT8_C(c))
#define UINT16_C(c) (__UINT16_C(c))
#define UINT32_C(c) (__UINT32_C(c))
#define UINT64_C(c) (__UINT64_C(c))

#define INTMAX_C(c)  (__INTMAX_C(c))
#define UINTMAX_C(c) (__UINTMAX_C(c))

#pragma clang diagnostic pop
#pragma clang diagnostic pop