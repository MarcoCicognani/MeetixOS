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
#include <errno.h>
#include <unistd.h>

char* getcwd(char* buf, usize size) {
    GetWorkingDirectoryStatus stat = s_get_working_directory_l(buf, size);
    if ( stat == GET_WORKING_DIRECTORY_SUCCESSFUL )
        return buf;
    else if ( stat == GET_WORKING_DIRECTORY_SIZE_EXCEEDED )
        errno = ERANGE;
    return NULL;
}
