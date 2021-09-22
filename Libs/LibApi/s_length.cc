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

i64 s_length(FileHandle fd) {
    return s_length_s(fd, 0);
}

i64 s_length_s(FileHandle fd, FsLengthStatus* out_status) {
    SyscallFsLength data{ SYSCALL_FS_LENGTH_BY_FD, nullptr, fd };
    do_syscall(SYSCALL_FS_LENGTH, (usize)&data);

    if ( out_status )
        *out_status = data.m_length_status;
    return data.m_length;
}

i64 s_flength(const char* path) {
    return s_flength_ss(path, true, nullptr);
}

i64 s_flength_s(const char* path, bool follow_symlinks) {
    return s_flength_ss(path, follow_symlinks, nullptr);
}

i64 s_flength_ss(const char* path, bool follow_symlinks, FsLengthStatus* out_status) {
    auto symlink_flag = follow_symlinks ? SYSCALL_FS_LENGTH_FOLLOW_SYMLINKS
                                        : SYSCALL_FS_LENGTH_NOT_FOLLOW_SYMLINKS;

    SyscallFsLength data{ symlink_flag | SYSCALL_FS_LENGTH_BY_PATH, path };
    do_syscall(SYSCALL_FS_LENGTH, (usize)&data);

    if ( out_status )
        *out_status = data.m_length_status;
    return data.m_length;
}
