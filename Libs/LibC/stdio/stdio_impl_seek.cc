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

#include <unistd.h>

int stdio_impl_seek(FILE* stream, off_t offset, int whence) {
    return static_cast<int>(lseek(stream->m_fd, offset, whence));
}
