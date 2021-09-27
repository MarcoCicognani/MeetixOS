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

usize strftime(A_UNUSED char*            ptr,
               A_UNUSED usize            maxsize,
               A_UNUSED const char*      format,
               A_UNUSED const struct tm* time_ptr) {
    __NOT_IMPLEMENTED(strftime);
    return 0;
}
