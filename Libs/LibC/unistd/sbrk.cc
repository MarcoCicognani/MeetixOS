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

#include <Api.h>
#include <errno.h>
#include <unistd.h>

extern "C" void* sbrk(intptr_t increment) {
    if ( void* result;  s_set_break(increment, &result) )
        return result;

    /* the kernel or the process have finished the memory */
    errno = ENOMEM;
    return (void*)-1;
}
