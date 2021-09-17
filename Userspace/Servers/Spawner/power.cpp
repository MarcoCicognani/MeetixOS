/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "Power.hpp"

#include <eva/utils/local.hpp>
#include <signal.h>
#include <stdio.h>
#include <tasking/tasking.hpp>

/**
 *
 */
static inline bool compare(const Pid& first, const Pid& second) {
    return first < second;
}

/**
 *
 */
void processHaltMachine(int command) {
    // get process tids and sort it
    auto process = Tasking::getProcessIDs();
    std::sort(process.begin(), process.end(), compare);

    // send SIGINT signal to all process
    for (auto& proc_id : process) {
        klog("sending SIGINT to thread %i", proc_id);
        RaiseSignal(proc_id, SIGINT);
    }

    // shutdown or reboot system
    if ( command == SPAWN_COMMAND_SHUTDOWN_MACHINE )
        shutDownMachine();
    else if ( command == SPAWN_COMMAND_REBOOT_MACHINE )
        reBootMachine();
}

/**
 *
 */
void shutDownMachine() { /* TODO */
}

/**
 *
 */
void reBootMachine() { /* TODO */
}
