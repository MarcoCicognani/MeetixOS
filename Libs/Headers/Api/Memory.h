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

#include <Api/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Paging page size
 */
#define PAGE_SIZE       0x1000
#define PAGE_ALIGN_MASK (PAGE_SIZE - 1)

/**
 * @brief Page alignment macros
 */
#define PAGE_ALIGN_DOWN(value) ((value) & ~(PAGE_SIZE - 1))
#define PAGE_ALIGN_UP(value)   (((value) & (PAGE_SIZE - 1)) ? (PAGE_ALIGN_DOWN((value)) + PAGE_SIZE) : (value))

/**
 * @brief Recursive paging macros
 */
#define TABLE_IN_DIRECTORY_INDEX(address) ((usize)(((address) / PAGE_SIZE) / 1024))
#define PAGE_IN_TABLE_INDEX(address)      ((usize)(((address) / PAGE_SIZE) % 1024))

#ifdef __cplusplus
}
#endif
