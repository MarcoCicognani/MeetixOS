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
#include <stdint.h>
#include <sys/types.h>

__BEGIN_C

/* ------------------------------------------- C types ------------------------------------------ */

typedef struct DIR {
    FsDirectoryIterator* m_directory_iterator;
    struct dirent*       m_entry_buffer;
} DIR;

__END_C
