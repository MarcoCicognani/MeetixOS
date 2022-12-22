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
    unsigned int m_seconds;
    unsigned int m_minutes;
    unsigned int m_hours;
    unsigned int m_month_day;
    unsigned int m_month;
    unsigned int m_year;
    unsigned int m_week_day;
    unsigned int m_year_day;
} A_PACKED DateTime;

#ifdef __cplusplus
}
#endif