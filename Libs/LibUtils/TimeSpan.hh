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

#include <Api/Types.h>

namespace Utils {

/* ------------------------------------------ C++ types ----------------------------------------- */

class TimeSpan {
public:
    const u64 C_SECOND = 1000;
    const u64 C_MINUTE = 60000;
    const u64 C_HOUR   = 3600000;
    const u64 C_DAY    = 216000000;

    u64 m_day{ 0 };
    u64 m_hour{ 0 };
    u64 m_minute{ 0 };
    u64 m_second{ 0 };
    u64 m_millisecond{ 0 };

    /**
     * @brief Constructors
     */
    TimeSpan() = default;
    TimeSpan(u64 day, u64 hour, u64 minute, u64 second, u64 millisecond)
        : m_day{ day }, m_hour{ hour }, m_minute{ minute }, m_second{ second }, m_millisecond{
              millisecond
          } {
    }

    bool operator==(const TimeSpan& rhs) const {
        return m_day == rhs.m_day && m_hour == rhs.m_hour && m_minute == rhs.m_minute
            && m_second == rhs.m_second && m_millisecond == rhs.m_millisecond;
    }
    bool operator!=(const TimeSpan& rhs) const {
        return m_day != rhs.m_day || m_hour != rhs.m_hour || m_minute != rhs.m_minute
            || m_second != rhs.m_second || m_millisecond != rhs.m_millisecond;
    }

    /**
     * @return the total time in milliseconds
     */
    [[nodiscard]] u64 as_millis() const {
        return (m_millisecond + (m_second * C_SECOND) + (m_minute * C_MINUTE) + (m_hour * C_HOUR)
                + (m_day * C_DAY));
    }

    /**
     * reset the internal values
     */
    void reset() {
        m_day = m_hour = m_minute = m_second = m_millisecond = 0;
    }
};

} /* namespace Utils */