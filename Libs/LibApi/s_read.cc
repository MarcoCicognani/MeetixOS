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

usize s_read(FileHandle fd, void* buffer, usize buffer_len) {
    return s_read_s(fd, buffer, buffer_len, nullptr);
}

usize s_read_s(FileHandle fd, void* buffer, usize buffer_len, FsReadStatus* out_status) {
    SyscallFsRead data{ fd, reinterpret_cast<u8*>(buffer), buffer_len };
    do_syscall(SYSCALL_FS_READ, (usize)&data);

    if ( out_status )
        *out_status = data.m_read_status;
    return data.m_read_bytes;
}
