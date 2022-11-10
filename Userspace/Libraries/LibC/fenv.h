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
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

TYPE_ALIAS(fexcept_t, uint16_t);
TYPE_ALIAS(fenv_t, struct fenv_t);

CONST_VALUE(FE_TONEAREST, int, 0);
CONST_VALUE(FE_DOWNWARD, int, 1);
CONST_VALUE(FE_UPWARD, int, 2);
CONST_VALUE(FE_TOWARDZERO, int, 3);
CONST_VALUE(FE_INVALID, int, 1u << 0);
CONST_VALUE(FE_DIVBYZERO, int, 1u << 2);
CONST_VALUE(FE_OVERFLOW, int, 1u << 3);
CONST_VALUE(FE_UNDERFLOW, int, 1u << 4);
CONST_VALUE(FE_INEXACT, int, 1u << 5);
CONST_VALUE(FE_ALL_EXCEPT, int, FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW);

#define FE_DFL_ENV ((const fenv_t*)-1)

struct __x87_floating_point_environment {
    uint16_t __control_word;
    uint16_t : 16;
    uint16_t __status_word;
    uint16_t : 16;
    uint16_t __tag_word;
    uint16_t : 16;
    uint32_t __fpu_ip_offset;
    uint16_t __fpu_ip_selector;
    uint16_t __opcode : 11;
    uint16_t          : 5;
    uint32_t __fpu_data_offset;
    uint16_t __fpu_data_selector;
    uint16_t : 16;
};
static_assert(sizeof(__x87_floating_point_environment) == 28);

struct fenv_t {
    struct __x87_floating_point_environment __x87_fpu_env;

    uint32_t __mxcsr;
};

int fegetenv(fenv_t*);
int fesetenv(const fenv_t*);
int feholdexcept(fenv_t*);
int feupdateenv(const fenv_t*);

int fegetexceptflag(fexcept_t*, int);
int fesetexceptflag(const fexcept_t*, int);

int feclearexcept(int);
int fetestexcept(int);
int feraiseexcept(int);

int fesetround(int);
int fegetround(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
