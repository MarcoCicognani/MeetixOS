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
 * @brief Date-Time descriptor
 */
typedef struct {
    u32 m_seconds;
    u32 m_minutes;
    u32 m_hours;
    u32 m_month_day;
    u32 m_month;
    u32 m_year;
    u32 m_week_day;
    u32 m_year_day;
} A_PACKED DateTime;

#ifdef __cplusplus
}
#endif