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
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#ifdef __cplusplus
extern "C" {
#endif

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

TYPE_ALIAS(jmp_buf, __JMP_BUF_TYPE[__JMP_BUF_LENGTH]);

int  setjmp(jmp_buf);
void longjmp(jmp_buf, int) A_NORETURN;

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
