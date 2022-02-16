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
#include <Api/StdInt.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#if __x86_64__
#    error "not implemented for architecture x86_64"
#elif __i386__
#    define FE_INVALID    1     /* Invalid argument exception */
#    define __FE_DENORM   2     /* Inexact result exception */
#    define FE_DIVBYZERO  4     /* Pole error exception */
#    define FE_OVERFLOW   8     /* Overflow range error exception */
#    define FE_UNDERFLOW  16    /* Underflow range error exception */
#    define FE_INEXACT    32    /* Inexact result exception */
#    define FE_ALL_EXCEPT 63    /* All exception */
#    define FE_TONEAREST  0     /* To-nearest rounding direction mode */
#    define FE_DOWNWARD   0x400 /* Downward rounding direction mode */
#    define FE_UPWARD     0x800 /* Upward rounding direction mode */
#    define FE_TOWARDZERO 0xc00 /* Toward-zero rounding direction mode */

/* ------------------------------------------- C types ------------------------------------------ */

typedef unsigned short fexcept_t;

typedef struct {
    u16 __control_word;
    u16 __unused1;
    u16 __status_word;
    u16 __unused2;
    u16 __tags;
    u16 __unused3;
    u32 __eip;
    u16 __cs_selector;
    u32 __opcode  : 11;
    u32 __unused4 : 5;
    u32 __data_offset;
    u16 __data_selector;
    u16 __unused5;
} fenv_t;

#    define FE_DFL_ENV ((const fenv_t*)-1)

#else
#    error "current architecture not supported"
#endif

/* ------------------------------------ C function prototypes ----------------------------------- */

int feclearexcept(int);
int fegetexceptflag(fexcept_t*, int);
int feraiseexcept(int);
int fesetexceptflag(const fexcept_t*, int);
int fetestexcept(int);
int fegetround(void);
int fesetround(int);
int fegetenv(fenv_t*);
int feholdexcept(fenv_t*);
int fesetenv(const fenv_t*);
int feupdateenv(const fenv_t*);

#ifdef __cplusplus
}
#endif
