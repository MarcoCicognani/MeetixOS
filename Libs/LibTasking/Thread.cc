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

namespace Tasking {

Thread::~Thread() {
    if (is_running())
        s_kill(m_thread_id);
}

void Thread::execute(Thread* thread) {
    thread->run();
}

Tid Thread::start() {
    return (m_thread_id = s_create_thread_d(reinterpret_cast<void*>(execute), this));
}

} /* namespace Tasking */
