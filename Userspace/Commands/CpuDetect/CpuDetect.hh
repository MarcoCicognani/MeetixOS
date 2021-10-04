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

#pragma once

#include <cstdint>

/**
 * @brief Executes the cpuid instruction
 */
#define CPU_ID(in, a, b, c, d) asm volatile("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(in))

/**
 * @brief Prints the string inside the given registers
 */
void print_regs(usize eax, usize ebx, usize ecx, usize edx);