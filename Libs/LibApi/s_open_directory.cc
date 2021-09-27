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

FsDirectoryIterator* s_open_directory(const char* path) {
    return s_open_directory_s(path, nullptr);
}

FsDirectoryIterator* s_open_directory_s(const char* path, FsOpenDirectoryStatus* out_status) {
    auto iterator                   = new FsDirectoryIterator{};
    iterator->m_entry_buffer.m_name = new char[FILENAME_MAX];

    SyscallFsOpenDirectory data{ path, iterator };
    do_syscall(SYSCALL_FS_OPEN_DIRECTORY, (usize)&data);

    if ( out_status )
        *out_status = data.m_open_directory_status;
    if ( data.m_open_directory_status == FS_OPEN_DIRECTORY_SUCCESSFUL ) {
        return iterator;
    } else {
        s_close_directory(iterator);
        return nullptr;
    }
}
