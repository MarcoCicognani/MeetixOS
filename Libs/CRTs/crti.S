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

.section .init
.global _init
.type _init, @function
_init:
    push %ebp
    mov %esp, %ebp

.section .fini
.global _fini
.type _fini, @function
_fini:
    push %ebp
    mov %esp, %ebp
