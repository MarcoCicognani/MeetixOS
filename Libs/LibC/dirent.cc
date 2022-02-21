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

#include <Api/FileSystem.h>
#include <dirent.h>
#include <errno.h>
#include <LibTC/Assertions.hh>
#include <string.h>

extern "C" {

struct DirStream {
    FsDirectoryIterator* m_directory_iterator;
    struct dirent*       m_entry_buffer;
};

DIR* opendir(const char* path) {
    FsOpenDirectoryStatus open_status{};

    auto dir_it = s_open_directory_s(path, &open_status);
    if ( open_status == FS_OPEN_DIRECTORY_SUCCESSFUL )
        return new DIR{ dir_it, new dirent{} };
    else if ( open_status == FS_OPEN_DIRECTORY_NOT_FOUND )
        errno = ENOTDIR;
    else if ( open_status == FS_OPEN_DIRECTORY_ERROR )
        errno = EIO;

    return nullptr;
}

struct dirent* readdir(DIR* dir) {
    FsReadDirectoryStatus read_status{};

    auto dir_entry = s_read_directory_s(dir->m_directory_iterator, &read_status);
    if ( read_status == FS_READ_DIRECTORY_SUCCESSFUL ) {
        auto entry_buffer             = dir->m_entry_buffer;
        entry_buffer->d_fileno        = dir_entry->m_node_id;
        dir->m_entry_buffer->d_dev    = -1; /* TODO */
        dir->m_entry_buffer->d_namlen = strlen(dir_entry->m_name);
        dir->m_entry_buffer->d_reclen = -1; /* TODO */
        strcpy(entry_buffer->d_name, dir_entry->m_name);

        switch ( dir_entry->m_node_type ) {
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
            default:
                VERIFY_NOT_REACHED();
        }
        return entry_buffer;
    } else if ( read_status == FS_READ_DIRECTORY_EOD )
        return nullptr;
    else if ( read_status == FS_READ_DIRECTORY_ERROR )
        errno = EIO;
    return nullptr;
}

int closedir(DIR* dir) {
    s_close_directory(dir->m_directory_iterator);
    delete dir->m_entry_buffer;
    delete dir;
    return 0;
}
}