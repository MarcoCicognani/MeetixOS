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

i64 s_seek(FileHandle fd, i64 off, FsSeekMode mode) {
    return s_seek_s(fd, off, mode, nullptr);
}

i64 s_seek_s(FileHandle fd, i64 off, FsSeekMode mode, FsSeekStatus* out_status) {
    SyscallFsSeek data{ fd, off, mode };
    do_syscall(SYSCALL_FS_SEEK, (usize)&data);

    if ( out_status )
        *out_status = data.m_seek_status;
    return data.m_result;
}
