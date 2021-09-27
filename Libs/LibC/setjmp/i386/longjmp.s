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

.global __longjmp
.global _longjmp
.global longjmp
.type __longjmp,@function
.type _longjmp,@function
.type longjmp,@function

__longjmp:
_longjmp:
longjmp:
    mov 4(%esp), %edx
    mov 8(%esp), %eax
    test %eax, %eax
    jnz 1f
    inc %eax
1:
    mov (%edx), %ebx
    mov 4(%edx), %esi
    mov 8(%edx), %edi
    mov 12(%edx), %ebp
    mov 16(%edx), %ecx
    mov %ecx, %esp
    mov 20(%edx), %ecx
    jmp *%ecx
