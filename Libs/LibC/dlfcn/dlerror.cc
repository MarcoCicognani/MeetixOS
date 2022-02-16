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

#include <dlfcn.h>

static char g_dl_error_string[512] = "dlfcn not available";

extern "C" A_WEAK char* dlerror(void) {
    return g_dl_error_string;
}
