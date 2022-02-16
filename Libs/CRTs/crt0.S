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

/* Symbol must be visible to the outside */
.global _start

/* <libc_main> and <s_exit> are provided by the library */
.extern libc_main
.extern s_exit

/* The <_start> function is where the kernel starts the execution */
_start:
    call libc_main
    push -1
    call s_exit
