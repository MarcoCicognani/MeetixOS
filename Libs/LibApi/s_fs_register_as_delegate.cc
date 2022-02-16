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

FsRegisterAsDelegateStatus s_fs_register_as_delegate(const char* name,
                                                     FsPhysID    phys_mountpoint_id,
                                                     FsVirtID*   out_mountpoint_id,
                                                     Address*    out_transaction_storage) {
    SyscallFsRegisterAsDelegate data{ name, phys_mountpoint_id };
    do_syscall(SYSCALL_FS_REGISTER_AS_DELEGATE, (usize)&data);

    if ( out_mountpoint_id )
        *out_mountpoint_id = data.m_vfs_mountpoint_id;
    if ( out_transaction_storage )
        *out_transaction_storage = data.m_transaction_storage;
    return data.m_register_as_delegate_status;
}
