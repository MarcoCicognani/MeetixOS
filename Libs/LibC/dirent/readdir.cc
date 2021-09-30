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

#include <Api.h>
#include <cerrno>
#include <cstring>
#include <dirent.h>

extern "C" struct dirent* readdir(DIR* dir) {
    FsReadDirectoryStatus stat;
    auto                  entry = s_read_directory_s(dir->m_directory_iterator, &stat);
    if ( stat == FS_READ_DIRECTORY_SUCCESSFUL ) {
        auto entry_buffer             = dir->m_entry_buffer;
        entry_buffer->d_fileno        = entry->m_node_id;
        dir->m_entry_buffer->d_dev    = -1; /* TODO */
        dir->m_entry_buffer->d_namlen = strlen(entry->m_name);
        dir->m_entry_buffer->d_reclen = -1; /* TODO */
        dir->m_entry_buffer->d_type   = -1; /* TODO */
        strcpy(entry_buffer->d_name, entry->m_name);
        return entry_buffer;
    } else if ( stat == FS_READ_DIRECTORY_EOD ) {
        return nullptr;
    } else if ( stat == FS_READ_DIRECTORY_ERROR ) {
        errno = EIO;
    }
    return nullptr;
}
