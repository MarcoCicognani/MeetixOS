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

#ifndef LIBC_BUILDING_LIBSTDCXX
#    include <Api.h>
#    include <dirent.h>
#    include <errno.h>

extern "C" DIR* opendir(const char* path) {
    FsOpenDirectoryStatus stat;
    auto                  iter = s_open_directory_s(path, &stat);
    if ( stat == FS_OPEN_DIRECTORY_SUCCESSFUL )
        return new DIR{ iter, new dirent{} };
    else if ( stat == FS_OPEN_DIRECTORY_NOT_FOUND )
        errno = ENOTDIR;
    else if ( stat == FS_OPEN_DIRECTORY_ERROR )
        errno = EIO;

    return nullptr;
}
#else
#    include <dirent.h>

extern "C" DIR* opendir(const char*) {
    return nullptr;
}
#endif
