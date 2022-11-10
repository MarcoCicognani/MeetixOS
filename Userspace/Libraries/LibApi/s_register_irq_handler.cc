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

#include "__internal.hh"

#include <Api/User.h>

RegisterIrqHandlerStatus s_register_irq_handler(u8 irq, void (*handler)(u8)) {
    SyscallRegisterIrqHandler data{ irq,
                                    reinterpret_cast<Address>(handler),
                                    reinterpret_cast<Address>(__restore_interrupted_state_callback) };
    do_syscall(SYSCALL_SIGNAL_IRQ_REGISTER_HANDLER, (usize)&data);
    return data.m_register_status;
}
