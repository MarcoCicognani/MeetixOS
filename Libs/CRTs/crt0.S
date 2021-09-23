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

# Symbol must be visible to the outside
.global _start

# <__main> and <Exit> are provided by the library
.extern __main
.extern s_exit

# The <_start> function is where the kernel starts the execution.
_start:
    call __main

# Endless loop, for the case that bad things happen
wait:
    hlt
    jmp wait
