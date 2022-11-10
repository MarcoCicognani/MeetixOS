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
#include <Api/Time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief s_time system call data
 */
typedef struct {
    DateTime* m_date_time;
    bool      m_result;
} A_PACKED SyscallDateTime;

#ifdef __cplusplus
}
#endif