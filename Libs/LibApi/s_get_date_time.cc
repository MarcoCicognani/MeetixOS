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

bool s_get_date_time(DateTime* date_time) {
    SyscallDateTime data{ date_time, false };
    do_syscall(SYSCALL_DATE_TIME_GET, (usize)&data);
    return data.m_result;
}