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

RaiseSignalStatus s_raise_signal(Pid proc_id, int signal) {
    SyscallRaiseSignal data{ signal, proc_id };
    do_syscall(SYSCALL_SIGNAL_RAISE, (usize)&data);
    return data.m_raise_status;
}
