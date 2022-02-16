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

#include <cstring>

extern "C" char* strdup(const char* src) {
    auto src_len = strlen(src);
    auto dest    = new char[src_len + 1];

    strncpy(dest, src, src_len);
    return dest;
}
