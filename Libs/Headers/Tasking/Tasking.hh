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
#include <vector>

namespace Tasking {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Returns a vector with the thread-ids of all the running threads
 */
std::vector<Tid> list_thread_ids();

/**
 * @brief Returns a vector with the process-ids of all the running processes
 */
std::vector<Pid> list_process_ids();

/**
 * @brief Returns a vector with the ThreadDescriptors of all the running threads
 */
std::vector<ThreadDescriptor> list_threads();

/**
 * @brief Returns a vector with the ProcessDescriptors of all the running processes
 */
std::vector<ProcessDescriptor> list_processes();

} /* namespace Tasking */
