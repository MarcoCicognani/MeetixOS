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

#include <Api.h>
#include <Tasking/Thread.hh>
#include <Tasking/Timer.hh>

namespace Tasking {

class TimerThread final : public Tasking::Thread {
public:
    TimerThread(const Utils::TimeSpan& time_span, std::function<void()>& function)
        : Tasking::Thread{}
        , m_time_span{ time_span }
        , m_function{ function } {
    }

    virtual ~TimerThread() = default;

    void stop() {
        m_lock.lock();
    }

    void restart() {
        m_lock.unlock();
    }

protected:
    [[noreturn]] void run() override {
        while ( true ) {
            m_function();
            s_sleep(m_time_span.as_millis());
            m_lock.lock();
        }
    }

private:
    Utils::TimeSpan       m_time_span{};
    std::function<void()> m_function{};
    Lock                  m_lock{};
};

Timer::Timer(const Utils::TimeSpan& time_span, std::function<void()> function, bool start_now)
    : m_timer_thread{ new TimerThread{ time_span, function } } {
    if ( start_now )
        start();
}

Timer::~Timer() {
    delete m_timer_thread;
}

bool Timer::start() {
    if ( !m_timer_thread->is_running() )
        return m_timer_thread->start() != -1;
    else
        return false;
}

void Timer::stop() {
    if ( m_timer_thread->is_running() )
        m_timer_thread->stop();
}

void Timer::restart() {
    if ( m_timer_thread->is_running() )
        m_timer_thread->restart();
}

} /* namespace Tasking */