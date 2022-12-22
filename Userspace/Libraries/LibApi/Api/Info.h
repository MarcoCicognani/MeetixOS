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
#include <Api/Kernel.h>
#include <Api/System.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Kernel name info
 */
typedef struct {
    /* Pre-Kernel loader name and version */
    char         m_loader_name[32];
    unsigned int m_loader_major;
    unsigned int m_loader_minor;
    unsigned int m_loader_patch;

    /* Kernel name and version */
    char         m_kernel_name[32];
    unsigned int m_kernel_major;
    unsigned int m_kernel_minor;
    unsigned int m_kernel_sub;
    unsigned int m_kernel_patch;
} A_PACKED KernelName;

/**
 * @brief System information
 */
typedef struct {
    unsigned int m_cpu_count;
    char         m_cpu_vendor[32];
    unsigned int m_memory_total_amount;
    unsigned int m_memory_free_amount;
} A_PACKED SystemInfo;

/**
 * @brief PCI Get count data
 */
typedef struct {
    unsigned int m_count;
} A_PACKED SyscallGetPciDeviceCount;

/**
 * @brief PCI get device data
 */
typedef struct {
    unsigned int  m_list_pos;
    unsigned char m_is_valid;
    unsigned char m_dev_slot;
    unsigned char m_dev_bus;
    unsigned char m_dev_function;
    unsigned char m_vendor_id;
    unsigned char m_device_id;
    unsigned char m_class_code;
    unsigned char m_subclass_code;
    unsigned char m_program;
} A_PACKED PCIDeviceHeader;

/**
 * @brief Get process count data
 */
typedef struct {
    unsigned int m_count;
} A_PACKED SyscallGetProcessCount;

/**
 * @brief Get process id data
 */
typedef struct {
    Pid*         m_id_buffer;
    unsigned int m_id_buffer_len;
    unsigned int m_stored_ids_count;
} A_PACKED SyscallGetProcessIDs;

/**
 * @brief Get Thread count data
 */
typedef struct {
    ThreadType   m_thread_type;
    unsigned int m_count;
} A_PACKED SyscallGetThreadCount;

/**
 * @brief Get Thread id data
 */
typedef struct {
    Pid*         m_id_buffer;
    ThreadType   m_thread_type;
    unsigned int m_id_buffer_len;
    unsigned int m_stored_ids_count;
} A_PACKED SyscallGetThreadIDs;

/**
 * @brief Thread descriptor
 */
typedef struct {
    unsigned char found;

    Tid        m_tid;
    Pid        m_parent;
    ThreadType m_thread_type;
    char       m_identifier[STRING_IDENTIFIER_MAX_LENGTH];

    unsigned int m_thread_user_stack_size;

    unsigned char m_priority;
    unsigned int  m_wait_count;
    unsigned int  m_execution_time;
    unsigned int  m_scheduled_times;
} A_PACKED ThreadDescriptor;

/**
 * @brief Process descriptor
 */
typedef struct {
    ThreadDescriptor m_main_thread;
    char             m_source_path[PATH_MAX];
    unsigned int     m_heap_size;
    unsigned int     m_image_size;
} A_PACKED ProcessDescriptor;

#ifdef __cplusplus
}
#endif
