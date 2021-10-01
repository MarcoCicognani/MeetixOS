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

#ifndef LIBC_BUILDING_LIBSTDCXX
#    include <string.h>

char* strdup(const char* src) {
    auto src_len = strlen(src);
    auto dest    = new char[src_len + 1];

    strncpy(dest, src, src_len);
    return dest;
}
#else
#    include <string.h>

char* strdup(const char*) {
    return nullptr;
}
#endif
