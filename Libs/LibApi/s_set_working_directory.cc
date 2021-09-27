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

SetWorkingDirectoryStatus s_set_working_directory(const char* path) {
    return s_set_working_directory_p(path, nullptr);
}

SetWorkingDirectoryStatus s_set_working_directory_p(const char*               path,
                                                    ProcessCreationIdentifier process) {
    SyscallFsSetWorkingDirectory data{ path, process };
    do_syscall(SYSCALL_SET_WORKING_DIRECTORY, (usize)&data);
    return data.m_working_directory_status;
}
