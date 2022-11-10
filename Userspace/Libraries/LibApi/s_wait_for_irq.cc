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

#include <Api/User.h>

void s_wait_for_irq(u8 irq) {
    SyscallWaitForIrq data = { .m_irq = irq };
    do_syscall(SYSCALL_SIGNAL_IRQ_WAIT, (usize)&data);
}
