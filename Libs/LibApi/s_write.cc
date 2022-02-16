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

usize s_write(FileHandle fd, const void* buffer, usize buffer_len) {
    return s_write_s(fd, buffer, buffer_len, nullptr);
}

usize s_write_s(FileHandle fd, const void* buffer, usize buffer_len, FsWriteStatus* out_status) {
    SyscallFsWrite data{ fd, reinterpret_cast<const u8*>(buffer), buffer_len };
    do_syscall(SYSCALL_FS_WRITE, (usize)&data);

    if ( out_status )
        *out_status = data.m_write_status;
    return data.m_write_bytes;
}
