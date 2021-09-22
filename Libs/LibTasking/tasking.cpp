/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <Api/utils/local.hpp>
#include <tasking/tasking.hpp>

/**
 * @return a list with tids of threads
 */
std::vector<Tid> Tasking::getThreadIDs() {
    // get the thread count
    std::vector<Tid> tids;
    uint32_t         tcount = s_get_thread_count();

    // only if call success
    if ( tcount > 0 ) {
        // create a temporaty buffer
        Local<Tid> buffer(new Tid[tcount]);

        // get the tids and copy to the list
        uint32_t copied = s_get_thread_ids(buffer(), tcount);
        for ( int i = 0; i < copied; i++ )
            tids.push_back(buffer()[i]);
    }

    return tids;
}

/**
 * @return a list with pids of Processes
 */
std::vector<Pid> Tasking::getProcessIDs() {
    // get the thread count
    std::vector<Pid> pids;
    uint32_t         pcount = s_get_process_count();

    // only if call success
    if ( pcount > 0 ) {
        // create a temporaty buffer
        Local<Tid> buffer(new Tid[pcount]);

        // get the tids and copy to the list
        uint32_t copied = s_get_process_ids(buffer(), pcount);
        for ( int i = 0; i < copied; i++ )
            pids.push_back(buffer()[i]);
    }

    return pids;
}

/**
 * @return list with descriptors of process
 */
std::vector<ProcessDescriptor> Tasking::getProcess() {
    // create a local list to store descriptors and get the pids
    std::vector<ProcessDescriptor> descriptors;
    std::vector<Pid>               proc_ids = Tasking::getProcessIDs();

    // check validity
    if ( !proc_ids.empty() ) {
        // get all descriptors by thread IDs
        for ( auto& pid : proc_ids ) {
            ProcessDescriptor process;
            if ( s_get_process_descriptor(pid, &process) )
                descriptors.push_back(process);
        }
    }

    return descriptors;
}

/**
 * @return list with descriptors of threads
 */
std::vector<ThreadDescriptor> Tasking::getThreads() {
    // create a local list to store descriptors and get the pids
    std::vector<ThreadDescriptor> descriptors;
    std::vector<Pid>              tids = Tasking::getThreadIDs();

    // check validity
    if ( !tids.empty() ) {
        // get all descriptors by tids
        for ( auto& tid : tids ) {
            ThreadDescriptor thread;
            if ( s_get_thread_descriptor(tid, &thread) )
                descriptors.push_back(thread);
        }
    }

    return descriptors;
}
