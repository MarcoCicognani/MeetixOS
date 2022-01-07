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

extern "C" int closedir(DIR* dir) {
    s_close_directory(dir->m_directory_iterator);
    delete dir->m_entry_buffer;
    delete dir;
    return 0;
}
