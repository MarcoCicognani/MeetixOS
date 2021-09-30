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
#include <dirent.h>

extern "C" DIR* opendir(const char* path) {
    FsOpenDirectoryStatus stat;
    auto                  iter = s_open_directory_s(path, &stat);
    if ( stat == FS_OPEN_DIRECTORY_SUCCESSFUL ) {
        auto dir                  = new DIR{};
        dir->m_entry_buffer       = new dirent{};
        dir->m_directory_iterator = iter;
        return dir;
    } else if ( stat == FS_OPEN_DIRECTORY_NOT_FOUND ) {
        errno = ENOTDIR;
    } else if ( stat == FS_OPEN_DIRECTORY_ERROR ) {
        errno = EIO;
    }
    return nullptr;
}
