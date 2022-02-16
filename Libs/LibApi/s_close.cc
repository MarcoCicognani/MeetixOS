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

FsCloseStatus s_close(FileHandle fd) {
    SyscallFsClose data{ fd };
    do_syscall(SYSCALL_FS_CLOSE, (usize)&data);
    return data.m_close_status;
}
