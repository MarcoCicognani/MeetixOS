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

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief s_log system call data
 */
typedef struct {
    const char* m_message_buffer;
} A_PACKED SyscallLog;

/**
 * @brief s_set_video_log system call data
 */
typedef struct {
    bool m_enable;
} A_PACKED SyscallSetVideoLog;

#ifdef __cplusplus
}
#endif
