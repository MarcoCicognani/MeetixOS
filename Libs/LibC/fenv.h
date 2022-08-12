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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FE_TONEAREST  0
#define FE_DOWNWARD   1
#define FE_UPWARD     2
#define FE_TOWARDZERO 3
#define FE_INVALID    1u << 0
#define FE_DIVBYZERO  1u << 2
#define FE_OVERFLOW   1u << 3
#define FE_UNDERFLOW  1u << 4
#define FE_INEXACT    1u << 5
#define FE_ALL_EXCEPT (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)
#define FE_DFL_ENV    ((const fenv_t*)-1)

typedef u16 fexcept_t;

struct __x87_floating_point_environment {
    u16 __control_word;
    u16 __reserved1;
    u16 __status_word;
    u16 __reserved2;
    u16 __tag_word;
    u16 __reserved3;
    u32 __fpu_ip_offset;
    u16 __fpu_ip_selector;
    u16 __opcode    : 11;
    u16 __reserved4 : 5;
    u32 __fpu_data_offset;
    u16 __fpu_data_selector;
    u16 __reserved5;
};

typedef struct fenv_t {
    struct __x87_floating_point_environment __x87_fpu_env;
    u32                                     __mxcsr;
} fenv_t;

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
}
#endif
