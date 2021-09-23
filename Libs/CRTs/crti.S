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

# GCC adds code after both of these sections for calling global constructors
# and destructors.

.section .init
.global _init
_init:
    push %ebp
    mov %esp, %ebp

.section .fini
.global _fini
_fini:
    push %ebp
    mov %esp, %ebp
