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

i64 s_tell(FileHandle fd) {
    return s_tell_s(fd, nullptr);
}

i64 s_tell_s(FileHandle fd, FsTellStatus* out_status) {
    SyscallFsTell data{ fd };
    do_syscall(SYSCALL_FS_TELL, (usize)&data);

    if ( out_status )
        *out_status = data.m_tell_status;
    return data.m_result;
}
