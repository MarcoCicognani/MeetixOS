/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_JOIN
#define EVA_MULTITASKING_WAIT_MANAGER_JOIN

#include <logger/logger.hpp>
#include <tasking/tasking.hpp>
#include <tasking/wait/waiter.hpp>

/**
 * This handler causes the task that it's appended to to store it's current state
 * and go somewhere else in the code.
 */
class WaiterPerformInterruption : public Waiter {
private:
    /**
     * internal data
     */
    uintptr_t entry;    // the interruption entry
    uintptr_t callback; // the callback point for restoring

public:
    /**
     * filled constructor
     *
     * @param entry:		the interruption entry
     * @param callback:		the callback point for restoring
     */
    WaiterPerformInterruption(uintptr_t entry, uintptr_t callback)
        : entry(entry), callback(callback) {
    }

    /**
     * implementation of check waiting method
     *
     * @param task:		the task that wait
     * @return true if task must keep waiting
     */
    virtual bool checkWaiting(Thread* task) {
        task->storeForInterruption();

        // set the new entry
        task->cpuState->eip = entry;

        // jump to next stack value
        uint32_t* esp = (uint32_t*)(task->cpuState->esp);

        // pass parameter value
        if ( task->interruptionInfo->type == ThreadInterruptionInfoType::IRQ ) {
            // pass IRQ
            --esp;
            *esp = task->interruptionInfo->handledIrq;
        }

        else if ( task->interruptionInfo->type == ThreadInterruptionInfoType::SIGNAL ) {
            // pass signal
            --esp;
            *esp = task->interruptionInfo->handledSignal;
        }

        // put callback as return address on stack
        --esp;
        *esp = callback;

        // set new ESP
        task->cpuState->esp = (uint32_t)esp;

        return false;
    }

    /**
     * @return the name of the waiter
     */
    virtual const char* debugName() {
        return "WaiterPerformInterruption";
    }
};

#endif
