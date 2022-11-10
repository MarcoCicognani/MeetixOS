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

FsCreateNodeStatus
s_fs_create_node(usize parent, const char* name, FsNodeType type, usize fs_id, usize* out_created_id) {
    SyscallFsCreateNode data{ parent, name, type, fs_id };
    do_syscall(SYSCALL_FS_CREATE_NODE, (usize)&data);

    if ( out_created_id )
        *out_created_id = data.m_vfs_node_id;
    return data.m_create_node_status;
}
