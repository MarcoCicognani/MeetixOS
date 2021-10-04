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
#include <dirent.h>
#include <errno.h>
#include <string.h>

extern "C" struct dirent* readdir(DIR* dir) {
    FsReadDirectoryStatus stat;
    auto                  entry = s_read_directory_s(dir->m_directory_iterator, &stat);
    if ( stat == FS_READ_DIRECTORY_SUCCESSFUL ) {
        auto entry_buffer             = dir->m_entry_buffer;
        entry_buffer->d_fileno        = entry->m_node_id;
        dir->m_entry_buffer->d_dev    = -1; /* TODO */
        dir->m_entry_buffer->d_namlen = strlen(entry->m_name);
        dir->m_entry_buffer->d_reclen = -1; /* TODO */
        strcpy(entry_buffer->d_name, entry->m_name);

        switch ( entry->m_node_type ) {
            case FS_NODE_TYPE_NONE:
            case FS_NODE_TYPE_ROOT:
            case FS_NODE_TYPE_FILE:
            case FS_NODE_TYPE_PIPE:
                dir->m_entry_buffer->d_type = DT_REG;
                break;
            case FS_NODE_TYPE_FOLDER:
            case FS_NODE_TYPE_MOUNTPOINT:
                dir->m_entry_buffer->d_type = DT_DIR;
                break;
        }
        return entry_buffer;
    } else if ( stat == FS_READ_DIRECTORY_EOD ) {
        return nullptr;
    } else if ( stat == FS_READ_DIRECTORY_ERROR ) {
        errno = EIO;
    }
    return nullptr;
}
