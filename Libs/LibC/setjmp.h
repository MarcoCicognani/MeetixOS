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

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#ifdef __x86_64__
#    define __JMP_BUF_LENGTH 8
#    define __JMP_BUF_TYPE   unsigned long long
#elif defined(__i386__)
#    define __JMP_BUF_LENGTH 9
#    define __JMP_BUF_TYPE   unsigned long
#else
#    error "architecture not supported"
#endif

#ifndef __JMP_BUF_LENGTH
#    error "no architecture-specific 'jmp_buf' length defined"
#endif
#ifndef __JMP_BUF_TYPE
#    error "no architecture-specific 'jmp_buf' type defined"
#endif

/* ------------------------------------------- C types ------------------------------------------ */

typedef __JMP_BUF_TYPE jmp_buf[__JMP_BUF_LENGTH];

/* ------------------------------------ C function prototypes ----------------------------------- */

int  setjmp(jmp_buf);
void longjmp(jmp_buf, int) A_NORETURN;

#ifdef __cplusplus
}
#endif
