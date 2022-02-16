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

#include <Api.h>
#include <string>

namespace Tasking {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Base class for thread OOP wrapper
 */
class Thread {
public:
    Thread() = default;
    Thread(const std::string& thread_name)
        : m_thread_id{ -1 }
        , m_thread_name{ thread_name } {}

    virtual ~Thread();

    /**
     * @brief Starts the Thread
     */
    Tid start();

    /**
     * @brief Returns whether this thread is running
     */
    [[nodiscard]] bool is_running() const { return m_thread_id != -1; }

protected:
    /**
     * @brief Method to implement with the thread-body
     */
    virtual void run() = 0;

protected:
    Tid         m_thread_id{ -1 };
    std::string m_thread_name{};

private:
    static void execute_run(Thread* thread);
};

} /* namespace Tasking */