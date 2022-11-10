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
#pragma ide diagnostic   ignored "modernize-deprecated-headers"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(FLT_RADIX, int, 2);
CONST_VALUE(DECIMAL_DIG, int, 21);
CONST_VALUE(FLT_DECIMAL_DIG, int, 9);
CONST_VALUE(DBL_DECIMAL_DIG, int, 17);
CONST_VALUE(LDBL_DECIMAL_DIG, int, 21);
CONST_VALUE(FLT_MIN, float, 1.17549e-38);
CONST_VALUE(DBL_MIN, double, 2.22507e-308);
CONST_VALUE(LDBL_MIN, long double, 3.3621e-4932L);
CONST_VALUE(FLT_TRUE_MIN, float, 1.4013e-45);
CONST_VALUE(DBL_TRUE_MIN, double, 4.94066e-324);
CONST_VALUE(LDBL_TRUE_MIN, long double, 3.6452e-4951L);
CONST_VALUE(FLT_MAX, float, 3.40282e+38);
CONST_VALUE(DBL_MAX, double, 1.79769e+308);
CONST_VALUE(LDBL_MAX, long double, 1.18973e+4932L);
CONST_VALUE(FLT_EPSILON, float, 1.19209e-07);
CONST_VALUE(DBL_EPSILON, double, 2.22045e-16);
CONST_VALUE(LDBL_EPSILON, long double, 1.0842e-19L);
CONST_VALUE(FLT_DIG, int, 6);
CONST_VALUE(DBL_DIG, int, 15);
CONST_VALUE(LDBL_DIG, int, 18);
CONST_VALUE(FLT_MANT_DIG, int, 24);
CONST_VALUE(DBL_MANT_DIG, int, 53);
CONST_VALUE(LDBL_MANT_DIG, int, 64);
CONST_VALUE(FLT_MIN_EXP, int, -125);
CONST_VALUE(DBL_MIN_EXP, int, -1021);
CONST_VALUE(LDBL_MIN_EXP, int, -16381);
CONST_VALUE(FLT_MIN_10_EXP, int, -37);
CONST_VALUE(DBL_MIN_10_EXP, int, -307);
CONST_VALUE(LDBL_MIN_10_EXP, int, -4931);
CONST_VALUE(FLT_MAX_EXP, int, 128);
CONST_VALUE(DBL_MAX_EXP, int, 1024);
CONST_VALUE(LDBL_MAX_EXP, int, 16384);
CONST_VALUE(FLT_MAX_10_EXP, int, 38);
CONST_VALUE(DBL_MAX_10_EXP, int, 308);
CONST_VALUE(LDBL_MAX_10_EXP, int, 4932);

#define FLT_ROUNDS (fegetround()) /* Note: this not might be true for non-x86 platforms */

CONST_VALUE(FLT_HAS_SUBNORM, bool, 1);
CONST_VALUE(DBL_HAS_SUBNORM, bool, 1);
CONST_VALUE(LDBL_HAS_SUBNORM, bool, 1);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
