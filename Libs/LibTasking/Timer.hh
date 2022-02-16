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

#include <functional>
#include <LibTasking/Lock.hh>
#include <LibUtils/TimeSpan.hh>
#include <string>

namespace Tasking {

/* ------------------------------------------ C++ types ----------------------------------------- */

class TimerThread;

/**
 * @brief Executes a function is a separate thread every tick of the given time-span
 */
class Timer {
public:
    /**
     * @brief Filled constructor
     */
    Timer(const Utils::TimeSpan& time_span, std::function<void()> function, bool start_now = false);
    ~Timer();

    /**
     * @brief Start the timer when is called, if called twice returns false
     */
    bool start();

    /**
     * @brief Stops the timer
     */
    void stop();

    /**
     * @brief Restarts the previously stopped timer
     */
    void restart();

private:
    TimerThread* m_timer_thread{};
};

} /* namespace Tasking */
