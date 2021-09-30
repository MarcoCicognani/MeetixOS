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

#include <Api/Common.h>

extern "C" {
void __cxa_atexit(A_UNUSED void (*func)(void*), A_UNUSED void* arg, A_UNUSED void* dso_handle) {
}
}
