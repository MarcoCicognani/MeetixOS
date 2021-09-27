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

FileHandle s_clone_fd(FileHandle source_fd, Pid source_proc_id, Pid target_proc_id) {
    return s_clone_fd_ts(source_fd, source_proc_id, -1, target_proc_id, 0);
}

FileHandle s_clone_fd_s(FileHandle       source_fd,
                        Pid              source_proc_id,
                        Pid              target_proc_id,
                        FsCloneFdStatus* out_status) {
    return s_clone_fd_ts(source_fd, source_proc_id, -1, target_proc_id, out_status);
}

FileHandle
s_clone_fd_t(FileHandle source_fd, Pid source_proc_id, FileHandle target_fd, Pid target_proc_id) {
    return s_clone_fd_ts(source_fd, source_proc_id, target_fd, target_proc_id, 0);
}

FileHandle s_clone_fd_ts(FileHandle       source_fd,
                         Pid              source_proc_id,
                         FileHandle       target_fd,
                         Pid              target_proc_id,
                         FsCloneFdStatus* out_status) {
    SyscallFsCloneFd data{ source_fd, source_proc_id, target_fd, target_proc_id };
    do_syscall(SYSCALL_FS_CLONEFD, (usize)&data);

    if ( out_status )
        *out_status = data.m_clone_fd_status;
    return data.m_cloned_fd;
}
