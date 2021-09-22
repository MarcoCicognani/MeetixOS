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

#include <Api/Kernel.h>

__BEGIN_C

/**
 * @brief s_call_vm_86 system call data
 */
typedef struct {
    usize          m_interrupt;
    VM86Registers  m_in_registers;
    VM86Registers* m_out_registers;
    Vm86CallStatus m_call_status;
} A_PACKED SyscallCallVm86;

__END_C
