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

#pragma once

#include <Api/Kernel.h>
#include <Api/StdInt.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief s_alloc_mem system call data
 */
typedef struct {
    usize m_region_size;
    void* m_region_ptr;
} A_PACKED SyscallAllocMem;

/**
 * @brief s_share_mem system call data
 */
typedef struct {
    void* m_region_ptr;
    usize m_size_to_share;
    Pid   m_target_proc_id;
    void* m_shared_ptr;
} A_PACKED SyscallShareMem;

/**
 * @brief s_map_mmio system call data
 */
typedef struct {
    void* m_physical_address;
    usize m_region_size;
    void* m_mapped_ptr;
} A_PACKED SyscallMapMmio;

/**
 * @brief s_unmap_mem system call data
 */
typedef struct {
    usize m_region_ptr;
} A_PACKED SyscallUnmap;

/**
 * @brief s_lower_malloc system call data
 */
typedef struct {
    usize m_region_size;
    void* m_region_ptr;
} A_PACKED SyscallLowerMalloc;

/**
 * @brief s_lower_free system call data
 */
typedef struct {
    void* m_region_ptr;
} A_PACKED SyscallLowerFree;

/**
 * @brief s_set_break system call data
 */
typedef struct {
    isize m_amount;
    usize m_out_address;
    bool  m_success;
} A_PACKED SyscallSbrk;

#ifdef __cplusplus
}
#endif
