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

#pragma once

#include <Api/Common.h>
#include <Api/FileSystem.h>
#include <dirent.h>

/* ------------------------------------------- C types ------------------------------------------ */

struct DirStream {
    FsDirectoryIterator* m_directory_iterator;
    struct dirent*       m_entry_buffer;
};
