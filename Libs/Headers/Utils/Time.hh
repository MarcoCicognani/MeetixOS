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

#include <Api.h>
#include <cstdint>

/* ------------------------------------------ C defines ----------------------------------------- */

#define TIME_DRIVER_IDENTIFIER "time"

namespace Utils::Time {

/* ------------------------------------------ C++ types ----------------------------------------- */

typedef enum
{
    CURRENT_TIME,
    CURRENT_UPTIME
} TimeDriverCommand;

struct TimeDriverHeader {
    TimeDriverCommand m_command{ CURRENT_TIME };

    TimeDriverHeader() = default;
} A_PACKED;

struct Current {
    u16 m_second{ 0 };
    u16 m_minute{ 0 };
    u16 m_hour{ 0 };
    u16 m_day{ 0 };
    u16 m_month{ 0 };
    u16 m_year{ 0 };

    /**
     * @brief Constructors
     */
    Current() = default;
    Current(u16 second, u16 minute, u16 hour, u16 day, u16 month, u16 year)
        : m_second{ second }, m_minute{ minute }, m_hour{ hour }, m_day{ day }, m_month{ month },
          m_year{ year } {
    }
} A_PACKED;

struct UpTime {
    u64 m_second{ 0 };

    UpTime() = default;
} A_PACKED;

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Fills the <current> time descriptor with the current time
 */
bool current(Current& current);

/**
 * @brief Fills the <up_time> descriptor with the current system uptime
 */
bool up_time(UpTime& up_time);

} /* namespace Utils::Time */