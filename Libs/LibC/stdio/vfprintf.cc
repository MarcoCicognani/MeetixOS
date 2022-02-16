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

#include "stdio_internal.hh"

#include <stdio.h>

extern "C" int vfprintf(FILE* stream, const char* format, va_list arg_list) {
    s_atomic_lock(&stream->m_lock);
    auto res       = vfprintf_unlocked(stream, format, arg_list);
    stream->m_lock = false;
    return res;
}
