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
#include <malloc.h>
#include <string.h>

struct dirent* readdir(DIR* dir) {
    FsReadDirectoryStatus stat;
    FsDirectoryEntry*     entry = s_read_directory_s(dir->m_directory_iterator, &stat);

    if ( stat == FS_READ_DIRECTORY_SUCCESSFUL ) {
        dirent* ent                   = dir->m_entry_buffer;
        ent->d_fileno                 = entry->m_node_id;
        dir->m_entry_buffer->d_dev    = -1; // TODO
        dir->m_entry_buffer->d_namlen = strlen(entry->m_name);
        dir->m_entry_buffer->d_reclen = -1; // TODO
        dir->m_entry_buffer->d_type   = -1; // TODO
        strcpy(ent->d_name, entry->m_name);
        return ent;
    } else if ( stat == FS_READ_DIRECTORY_EOD ) {
        return NULL;
    } else if ( stat == FS_READ_DIRECTORY_ERROR ) {
        errno = EIO;
    }

    return NULL;
}
