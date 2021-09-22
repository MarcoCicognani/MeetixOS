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

FileHandle s_open(const char* path) {
    return s_open_fms(path, 0, 0, nullptr);
}

FileHandle s_open_f(const char* path, i32 flags) {
    return s_open_fms(path, flags, 0, nullptr);
}

FileHandle s_open_fs(const char* path, i32 flags, FsOpenStatus* out_status) {
    return s_open_fms(path, flags, 0, out_status);
}

FileHandle s_open_fms(const char* path, i32 flags, i32 mode, FsOpenStatus* out_status) {
    SyscallFsOpen data{ path, flags, mode };
    do_syscall(SYSCALL_FS_OPEN, (usize)&data);

    if ( out_status )
        *out_status = data.m_open_status;
    return data.m_open_fd;
}
