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

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-macro-to-enum"

#define USE_LOCKS 1 /* 1 - use internal spinlock implementation */
#define HAVE_MMAP 0 /* 0 - fallbacks to sbrk() implementation */

#define LACKS_SYS_MMAN_H 1

#define DEBUG   0
#define FOOTERS 0

#pragma clang diagnostic pop
