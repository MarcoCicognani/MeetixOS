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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Ramdisk s_spawn statuses
 */
typedef enum {
    RAMDISK_SPAWN_STATUS_SUCCESSFUL,
    RAMDISK_SPAWN_STATUS_FAILED_NOT_FOUND,
    RAMDISK_SPAWN_STATUS_FAILED_NOT_VALID,
    RAMDISK_SPAWN_STATUS_FAILED_NOT_PERMITTED
} RamdiskSpawnStatus;

/**
 * @brief Maximum length of paths within the ramdisk
 */
#define RAMDISK_MAXIMUM_PATH_LENGTH 512

/**
 * @brief Ramdisk entries types
 */
typedef enum {
    RAMDISK_ENTRY_TYPE_UNKNOWN = -1,
    RAMDISK_ENTRY_TYPE_FOLDER,
    RAMDISK_ENTRY_TYPE_FILE
} RamdiskEntryType;

/**
 * @brief Ramdisk entry information struct used within system calls
 */
typedef struct {
    RamdiskEntryType m_entry_type;
    char             m_name[512];
    usize            m_name_len;
} RamdiskEntryInfo;

#ifdef __cplusplus
}
#endif
