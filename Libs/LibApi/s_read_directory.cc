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

FsDirectoryEntry* s_read_directory(FsDirectoryIterator* iterator) {
    return s_read_directory_s(iterator, nullptr);
}

FsDirectoryEntry* s_read_directory_s(FsDirectoryIterator*   iterator,
                                     FsReadDirectoryStatus* out_status) {
    SyscallFsReadDirectory data = { .m_directory_iterator = iterator };
    do_syscall(SYSCALL_FS_READ_DIRECTORY, (usize)&data);

    if ( out_status )
        *out_status = data.m_read_directory_status;
    if ( data.m_read_directory_status == FS_READ_DIRECTORY_SUCCESSFUL )
        return &iterator->m_entry_buffer;
    else
        return nullptr;
}
