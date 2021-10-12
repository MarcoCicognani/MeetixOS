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

#include <calls/SyscallHandler.hpp>
#include <system/timing/RTC.hh>
#include <tasking/process.hpp>

SYSCALL_HANDLER(getDateTime) {
    auto data = reinterpret_cast<SyscallDateTime*>(SYSCALL_DATA(currentThread->cpuState));
    if ( !data->m_date_time )
        data->m_result = false;
    else {
        RTC::read(data->m_date_time);
        data->m_result = true;
    }
    return currentThread;
}