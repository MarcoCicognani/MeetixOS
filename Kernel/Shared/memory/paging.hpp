/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_SHARED_MEMORY_PAGING
#define EVA_SHARED_MEMORY_PAGING

#include "Api/Memory.h"
#include "Api/StdInt.h"

/**
 * Page directory flags
 */
const uint32_t PAGE_TABLE_PRESENT        = 1;
const uint32_t PAGE_TABLE_READWRITE      = 2;
const uint32_t PAGE_TABLE_USERSPACE      = 4;
const uint32_t PAGE_TABLE_WRITETHROUGH   = 8;
const uint32_t PAGE_TABLE_CACHE_DISABLED = 16;
const uint32_t PAGE_TABLE_ACCESSED       = 32;
const uint32_t PAGE_TABLE_SIZE           = 64;

/**
 * Page table flags
 */
const uint32_t PAGE_PRESENT        = 1;
const uint32_t PAGE_READWRITE      = 2;
const uint32_t PAGE_USERSPACE      = 4;
const uint32_t PAGE_WRITETHROUGH   = 8;
const uint32_t PAGE_CACHE_DISABLED = 16;
const uint32_t PAGE_ACCESSED       = 32;
const uint32_t PAGE_DIRTY          = 64;
const uint32_t PAGE_GLOBAL         = 128;

/**
 * Default flags
 */
#define DEFAULT_KERNEL_TABLE_FLAGS (PAGE_TABLE_PRESENT | PAGE_TABLE_READWRITE)
#define DEFAULT_KERNEL_PAGE_FLAGS  (PAGE_PRESENT | PAGE_READWRITE | PAGE_GLOBAL)
#define DEFAULT_USER_TABLE_FLAGS   (PAGE_TABLE_PRESENT | PAGE_TABLE_READWRITE | PAGE_TABLE_USERSPACE)
#define DEFAULT_USER_PAGE_FLAGS    (PAGE_PRESENT | PAGE_READWRITE | PAGE_USERSPACE)

/**
 * Typedefs for better readability
 */
typedef uint32_t* PageDirectory;
typedef uint32_t* PageTable;

/**
 * Invlpg
 */
#define INVLPG(addr) __asm__ __volatile__("invlpg (%%eax)" : : "a"(addr));

#endif
