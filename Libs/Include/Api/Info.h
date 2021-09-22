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

#include <Api/Common.h>
#include <Api/Kernel.h>
#include <Api/System.h>

__BEGIN_C

/**
 * @brief Kernel name info
 */
typedef struct {
    /* Pre-Kernel loader name and version */
    char  m_loader_name[32];
    usize m_loader_major;
    usize m_loader_minor;
    usize m_loader_patch;

    /* Kernel name and version */
    char  m_kernel_name[32];
    usize m_kernel_major;
    usize m_kernel_minor;
    usize m_kernel_sub;
    usize m_kernel_patch;
} A_PACKED KernelName;

/**
 * @brief System information
 */
typedef struct {
    usize m_cpu_count;
    char  m_cpu_vendor[32];
    usize m_memory_total_amount;
    usize m_memory_free_amount;
} A_PACKED SystemInfo;

/**
 * @brief PCI Get count data
 */
typedef struct {
    usize m_count;
} A_PACKED SyscallGetPciDeviceCount;

/**
 * @brief PCI get device data
 */
typedef struct {
    u32 m_list_pos;
    u8  m_is_valid;
    u8  m_dev_slot;
    u8  m_dev_bus;
    u8  m_dev_function;
    u16 m_vendor_id;
    u16 m_device_id;
    u8  m_class_code;
    u8  m_subclass_code;
    u8  m_program;
} A_PACKED PciDeviceHeader;

/**
 * @brief Get process count data
 */
typedef struct {
    usize m_count;
} A_PACKED SyscallGetProcessCount;

/**
 * @brief Get process id data
 */
typedef struct {
    Pid*  m_id_buffer;
    usize m_id_buffer_len;
    usize m_stored_ids_count;
} A_PACKED SyscallGetProcessIDs;

/**
 * @brief Get Thread count data
 */
typedef struct {
    ThreadType m_thread_type;
    usize      m_count;
} A_PACKED SyscallGetThreadCount;

/**
 * @brief Get Thread id data
 */
typedef struct {
    Pid*       m_id_buffer;
    ThreadType m_thread_type;
    usize      m_id_buffer_len;
    usize      m_stored_ids_count;
} A_PACKED SyscallGetThreadIDs;

/**
 * @brief Thread descriptor
 */
typedef struct {
    u8 found;

    Tid        m_tid;
    Pid        m_parent;
    ThreadType m_thread_type;
    char       m_identifier[STRING_IDENTIFIER_MAX_LENGTH];

    usize m_thread_user_stack_size;

    u8    m_priority;
    usize m_wait_count;
    usize m_execution_time;
    usize m_scheduled_times;
} A_PACKED ThreadDescriptor;

/**
 * @brief Process descriptor
 */
typedef struct {
    ThreadDescriptor m_main_thread;
    char             m_source_path[PATH_MAX];
    usize            m_heap_size;
    usize            m_image_size;
} A_PACKED ProcessDescriptor;

__END_C
