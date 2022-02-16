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

void __restore_interrupted_state_callback() {
    s_restore_interrupted_state();
    __builtin_unreachable();
}

void s_restore_interrupted_state() {
    do_syscall(SYSCALL_SIGNAL_RESTORE_STATE, 0);
}
