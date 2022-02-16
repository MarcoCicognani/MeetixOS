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

#include <Api.h>
#include <LibTasking/Thread.hh>

namespace Tasking {

Thread::~Thread() {
    if ( is_running() )
        s_kill(m_thread_id);
}

void Thread::execute_run(Thread* thread) {
    thread->run();
}

Tid Thread::start() {
    m_thread_id = s_create_thread_dn(reinterpret_cast<void*>(execute_run), this, m_thread_name.c_str());
    return m_thread_id;
}

} /* namespace Tasking */
