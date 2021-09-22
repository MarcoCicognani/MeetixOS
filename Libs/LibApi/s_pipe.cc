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

void s_pipe(FileHandle* out_write, FileHandle* out_read) {
    return s_pipe_s(out_write, out_read, nullptr);
}

void s_pipe_s(FileHandle* out_write, FileHandle* out_read, FsPipeStatus* out_status) {
    SyscallFsPipe data;
    do_syscall(SYSCALL_FS_PIPE, (usize)&data);

    *out_write = data.m_write_end_fd;
    *out_read  = data.m_read_end_fd;
    if ( out_status )
        *out_status = data.m_pipe_status;
}
