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

void s_fs_set_transaction_status(FsTransactionID id, FsTransactionStatus status) {
    SyscallFsSetTransactionStatus data{ id, status };
    do_syscall(SYSCALL_FS_SET_TRANSACTION_STATUS, (usize)&data);
}
