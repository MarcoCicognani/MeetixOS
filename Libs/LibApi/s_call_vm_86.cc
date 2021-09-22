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

#include <Api/User.h>

Vm86CallStatus s_call_vm_86(usize interrupt, VM86Registers* in_regs, VM86Registers* out_regs) {
    SyscallCallVm86 data{ interrupt, *in_regs, out_regs };
    do_syscall(SYSCALL_PROCESS_CALL_VM86, (usize)&data);
    return data.m_call_status;
}
