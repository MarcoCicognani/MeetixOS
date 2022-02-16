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

#include <Api/utils/local.hpp>
#include <LibTasking/Tasking.hh>

namespace Tasking {

std::vector<Tid> list_thread_ids() {
    /* obtain the amount of the running threads */
    auto thread_count = s_get_thread_count();
    if ( thread_count ) {
        auto ids_buffer = Local{ new Tid[thread_count] };

        /* obtain all the identifier and put them all into the vector */
        auto ids_amount = s_get_thread_ids(ids_buffer(), thread_count);
        return { ids_buffer(), ids_buffer() + ids_amount };
    }

    return {};
}

std::vector<Pid> list_process_ids() {
    /* obtain the amount of the running threads */
    auto proc_count = s_get_process_count();
    if ( proc_count ) {
        auto ids_buffer = Local{ new Pid[proc_count] };

        /* obtain all the identifier and put them all into the vector */
        auto ids_amount = s_get_thread_ids(ids_buffer(), proc_count);
        return { ids_buffer(), ids_buffer() + ids_amount };
    }

    return {};
}

std::vector<ThreadDescriptor> list_threads() {
    auto thread_ids = list_thread_ids();
    if ( !thread_ids.empty() ) {
        std::vector<ThreadDescriptor> thread_descs;
        for ( auto& thread_id : thread_ids ) {
            /* obtain the thread descriptor for the current thread-id */
            ThreadDescriptor thread_desc;
            if ( s_get_thread_descriptor(thread_id, &thread_desc) ) {
                thread_descs.push_back(thread_desc);
            }
        }

        return thread_descs;
    }
    return {};
}

std::vector<ProcessDescriptor> list_processes() {
    auto proc_ids = list_process_ids();
    if ( !proc_ids.empty() ) {
        std::vector<ProcessDescriptor> proc_descs;
        for ( auto& proc_id : proc_ids ) {
            /* obtain the thread descriptor for the current process-id */
            ProcessDescriptor thread_desc;
            if ( s_get_process_descriptor(proc_id, &thread_desc) ) {
                proc_descs.push_back(thread_desc);
            }
        }

        return proc_descs;
    }
    return {};
}

} /* namespace Tasking */