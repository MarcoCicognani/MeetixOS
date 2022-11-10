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

GetWorkingDirectoryStatus s_get_working_directory(char* buffer) {
    return s_get_working_directory_l(buffer, 512 /* PATH_MAX */);
}

GetWorkingDirectoryStatus s_get_working_directory_l(char* buffer, usize buffer_len) {
    SyscallFsGetWorkingDirectory data{ buffer, buffer_len };
    do_syscall(SYSCALL_GET_WORKING_DIRECTORY, (usize)&data);
    return data.m_working_directory_status;
}
