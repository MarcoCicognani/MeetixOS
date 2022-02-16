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

#include "DIR.hh"

#include <Api.h>
#include <cerrno>
#include <dirent.h>

extern "C" DIR* opendir(const char* path) {
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
