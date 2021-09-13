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

#include <eva/utils/local.hpp>
#include <tasking/tasking.hpp>

/**
 * @return a list with tids of threads
 */
llist<Tid> Tasking::getThreadIDs() {
    // get the thread count
    llist<Tid> tids;
    uint32_t   tcount = GetThreadCount();

    // only if call success
    if ( tcount > 0 ) {
        // create a temporaty buffer
        Local<Tid> buffer(new Tid[tcount]);

        // get the tids and copy to the list
        uint32_t copied = GetThreadTids(buffer(), tcount);
        for ( int i = 0; i < copied; i++ )
            tids.add(buffer()[i]);
    }

    return tids;
}

/**
 * @return a list with pids of Processes
 */
llist<Pid> Tasking::getProcessIDs() {
    // get the thread count
    llist<Pid> pids;
    uint32_t   pcount = GetProcessCount();

    // only if call success
    if ( pcount > 0 ) {
        // create a temporaty buffer
        Local<Tid> buffer(new Tid[pcount]);

        // get the tids and copy to the list
        uint32_t copied = GetProcessPids(buffer(), pcount);
        for ( int i = 0; i < copied; i++ )
            pids.add(buffer()[i]);
    }

    return pids;
}

/**
 * @return list with descriptors of process
 */
llist<ProcessDescriptor> Tasking::getProcess() {
    // create a local list to store descriptors and get the pids
    llist<ProcessDescriptor> descriptors;
    llist<Pid>               pids = Tasking::getProcessIDs();

    // check validity
    if ( pids.count() > 0 ) {
        // get all descriptors by tids
        for ( llist<Pid>::iterator it = pids.begin(); it != pids.end(); it++ ) {
            ProcessDescriptor process;
            if ( GetProcessDescriptor(*it, &process) )
                descriptors.add(process);
        }
    }

    return descriptors;
}

/**
 * @return list with descriptors of threads
 */
llist<ThreadDescriptor> Tasking::getThreads() {
    // create a local list to store descriptors and get the pids
    llist<ThreadDescriptor> descriptors;
    llist<Pid>              tids = Tasking::getThreadIDs();

    // check validity
    if ( tids.count() > 0 ) {
        // get all descriptors by tids
        for ( llist<Pid>::iterator it = tids.begin(); it != tids.end(); it++ ) {
            ThreadDescriptor thread;
            if ( GetThreadDescriptor(*it, &thread) )
                descriptors.add(thread);
        }
    }

    return descriptors;
}
