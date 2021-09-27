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

#include <dlfcn.h>
#include <stdlib.h>

A_WEAK void* dlsym(A_UNUSED void* handler, A_UNUSED const char* symbol) {
    return NULL;
}