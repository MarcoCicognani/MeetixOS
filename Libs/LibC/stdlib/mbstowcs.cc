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

#include <LibC/stdlib.h>

extern "C" usize mbstowcs(wchar_t*, const char*, usize) {
    __NOT_IMPLEMENTED(mbstowcs);
    return 0;
}
