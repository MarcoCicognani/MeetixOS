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
#include <time.h>

char* strptime(A_UNUSED const char* buf, A_UNUSED const char* format, A_UNUSED struct tm* tm) {
    __NOT_IMPLEMENTED(strptime);
    return NULL;
}
