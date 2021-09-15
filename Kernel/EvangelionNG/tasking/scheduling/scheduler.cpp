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

#include "debug/DebugInterfaceKernel.hpp"

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/gdt/GdtManager.hpp>
#include <system/interrupts/lapic.hpp>
#include <tasking/scheduling/scheduler.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/wait/WaiterSleep.hpp>
#include <utils/string.hpp>

/**
 * Creates a new scheduler for the given core.
 *
 * @param coreId:		id of the core
 */
Scheduler::Scheduler(uint32_t coreId)
    : milliseconds(0), coreId(coreId), waitQueue(0), runQueue(0), idleEntry(0), currentEntry(0) {
}

/**
 * Adds the given thread to the run queue.
 *
 * @param thread:		the thread to schedule
 */
void Scheduler::add(Thread* newThread) {
    // set the scheduler on the task
    newThread->scheduler = this;

    // the idle task is not added to a queue
    if ( newThread->priority == THREAD_PRIORITY_IDLE ) {
        TaskEntry* entry = new TaskEntry();
        entry->value     = newThread;
        entry->next      = 0;
        idleEntry        = entry;
    }

    else {
        // add task to run queue
        TaskEntry* entry = new TaskEntry();
        entry->value     = newThread;
        entry->next      = runQueue;
        runQueue         = entry;
    }

    logDebug("%! task %i assigned to core %i", "scheduler", newThread->id, coreId);
}

/**
 * Saves the processor state to the thread structure that was
 * last executed and returns a pointer to this thread. If there is
 * no last thread, scheduling is performed.
 *
 * @param cpuState:		pointer to the CPU structure on the process stack
 * @return the last executed thread
 */
Thread* Scheduler::save(ProcessorState* cpuState) {
    // store processor state in current task
    if ( currentEntry ) {
        currentEntry->value->cpuState = cpuState;
        return currentEntry->value;
    }

    // no last thread? do scheduling
    return schedule();
}

/**
 * Performs the scheduling. This processes the waiting tasks and then
 * does the scheduling algorithm to pick the next task.
 *
 * @return the next task to execute
 */
Thread* Scheduler::schedule() {
    // store which entry is running
    TaskEntry* runningEntry = currentEntry;

    // process wait queue (overwrites current entry)
    _processWaitQueue();

    // continue scheduling there
    currentEntry = runningEntry;

    // select next task to run
    while ( true ) {
        // when scheduling for the first time, there's not current task
        if ( !currentEntry )
            currentEntry = runQueue;

        else {
            // select next in run queue
            currentEntry = currentEntry->next;
            if ( !currentEntry )
                currentEntry = runQueue;
        }

        // no task in run queue? select idle thread
        if ( !currentEntry ) {
            currentEntry = idleEntry;
            if ( !currentEntry )
                EvaKernel::panic("%! idle thread does not exist on core %i", "scheduler", coreId);
        }

        // sanity check
        if ( currentEntry->value->waitManager )
            EvaKernel::panic("task %i is in run queue had wait manager '%s'",
                             currentEntry->value->id,
                             currentEntry->value->waitManager->debugName());

        // try to switch
        _applyContextSwitch(currentEntry->value);

        // remove it task is no more alive
        if ( !_checkAliveState(currentEntry->value) ) {
            currentEntry = 0;
            continue;
        }

        // task was successfully selected & switched to
        break;
    }

    // finish the switch
    _finishSwitch(currentEntry->value);
    ++currentEntry->value->rounds;

    return currentEntry->value;
}

/**
 * Moves the given thread to the top of the wait queue so it is
 * processed as soon as possible.
 *
 * @param thread:		the thread to push
 */
void Scheduler::increaseWaitPriority(Thread* thread) {
    // remove entry from wait queue
    TaskEntry* entry = _removeFromQueue(&waitQueue, thread);

    // put it on top of it
    if ( entry ) {
        entry->next = waitQueue;
        waitQueue   = entry;
    }
}

/**
 * Generates a value that is used to representate the load for this
 * scheduler.
 *
 * @return a numeric load value
 */
uint32_t Scheduler::calculateLoad() {
    uint32_t load = 0;

    // TODO improve load calculation
    TaskEntry* entry = runQueue;
    while ( entry ) {
        ++load;
        entry = entry->next;
    }
    entry = waitQueue;
    while ( entry ) {
        ++load;
        entry = entry->next;
    }

    return load;
}

/**
 * @return the thread that was executed last
 */
Thread* Scheduler::lastThread() {
    if ( currentEntry )
        return currentEntry->value;
    return 0;
}

/**
 * Removes the given thread from the wait queue and adds it to the
 * run queue.
 *
 * @param thread:		the thread to move
 */
void Scheduler::moveToRunQueue(Thread* thread) {
    TaskEntry* moveEntry = _removeFromQueue(&waitQueue, thread);

    // entry is already in run queue
    if ( !moveEntry )
        return;

    // put to start of run queue
    moveEntry->next = runQueue;
    runQueue        = moveEntry;
}

/**
 * Removes the given thread from the run queue and adds it to the
 * wait queue.
 *
 * @param thread:		the thread to move
 */
void Scheduler::moveToWaitQueue(Thread* thread) {
    TaskEntry* moveEntry = _removeFromQueue(&runQueue, thread);

    // entry is already in wait queue
    if ( !moveEntry )
        return;

    // put to start of wait queue
    moveEntry->next = waitQueue;
    waitQueue       = moveEntry;

    // may no more be the running entry
    if ( moveEntry == currentEntry )
        currentEntry = 0;
}

/**
 * Sets all threads of the given process to not alive and returns
 * whether there where any threads that are still alive.
 *
 * @param process:		the process of which all threads shall be killed
 */
void Scheduler::removeThreads(Process* process) {
    // set all threads in run queue to dead
    TaskEntry* entry = runQueue;
    while ( entry ) {
        TaskEntry* next = entry->next;
        if ( entry->value->process->main->id == process->main->id )
            _remove(entry->value);

        entry = next;
    }

    // set all threads in wait queue to dead
    entry = waitQueue;
    while ( entry ) {
        TaskEntry* next = entry->next;
        if ( entry->value->process->main->id == process->main->id )
            _remove(entry->value);
        entry = next;
    }
}

/**
 * Is called by the timer interrupt to count time since the scheduler is
 * running. Increases the millisecond count by the amount of time that each
 * timer tick takes.
 */
void Scheduler::updateMilliseconds() {
    milliseconds += APIC_MILLISECONDS_PER_TICK;

#if DEBUG_THREAD_DUMPING
    // debug dump
    static uint64_t lastDebugoutMillis = 0;
    if ( milliseconds - lastDebugoutMillis > 10000 ) {
        lastDebugoutMillis = milliseconds;

        logInfo("----------------");
        TaskEntry* entry = runQueue;
        while ( entry ) {
            Thread* thr = entry->value;
            logInfo("%# running - %i:%i, eip: %h, waiter: %s, name: %s, rounds: %i",
                    thr->process->main->id,
                    thr->id,
                    thr->cpuState->eip,
                    (thr->waitManager == 0 ? "-" : thr->waitManager->debugName()),
                    (thr->getIdentifier() == 0 ? "-" : thr->getIdentifier()),
                    thr->rounds);
            entry = entry->next;
        }
        entry = waitQueue;
        while ( entry ) {
            Thread* thr = entry->value;
            logInfo("%# waiting - %i:%i, eip: %h, waiter: %s, name: %s, rounds: %i",
                    thr->process->main->id,
                    thr->id,
                    thr->cpuState->eip,
                    (thr->waitManager == 0 ? "-" : thr->waitManager->debugName()),
                    (thr->getIdentifier() == 0 ? "-" : thr->getIdentifier()),
                    thr->rounds);
            entry = entry->next;
        }
    }
#endif

#if DEBUG_INTERFACE_MODE == DEBUG_INTERFACE_MODE_FULL
    static uint64_t lastProcessorTimeUpdate = 0;
    if ( milliseconds - lastProcessorTimeUpdate > 500 ) {
        lastProcessorTimeUpdate = milliseconds;

        TaskEntry* entry = runQueue;
        while ( entry ) {
            Thread* thr = entry->value;
            DEBUG_INTERFACE_TASK_SET_ROUNDS(thr->id, thr->rounds);
            thr->rounds = 0;
            entry       = entry->next;
        }
        entry = waitQueue;
        while ( entry ) {
            Thread* thr = entry->value;
            DEBUG_INTERFACE_TASK_SET_ROUNDS(thr->id, thr->rounds);
            thr->rounds = 0;
            entry       = entry->next;
        }
    }
#endif
}

/**
 * Retrieves the task with the given id.
 *
 * @return the thread or 0 if not existant
 */
Thread* Scheduler::getTaskById(Tid id) {
    Thread* thread = 0;

    TaskEntry* entry = runQueue;
    while ( entry ) {
        if ( entry->value->alive && entry->value->id == id ) {
            thread = entry->value;
            break;
        }
        entry = entry->next;
    }

    if ( !thread ) {
        entry = waitQueue;
        while ( entry ) {
            if ( entry->value->alive && entry->value->id == id ) {
                thread = entry->value;
                break;
            }
            entry = entry->next;
        }
    }

    return thread;
}

/**
 * Retrieves the task with the given identifier.
 *
 * @param identifier:		the task identifier to find
 * @return the thread or 0 if not existant
 */
Thread* Scheduler::getTaskByIdentifier(const char* identifier) {
    Thread* thread = 0;

    TaskEntry* entry = runQueue;
    while ( entry ) {
        if ( entry->value->alive ) {
            const char* taskIdentifier = entry->value->getIdentifier();
            if ( taskIdentifier != 0 && String::equals(taskIdentifier, identifier) ) {
                thread = entry->value;
                break;
            }
        }
        entry = entry->next;
    }

    if ( !thread ) {
        entry = waitQueue;
        while ( entry ) {
            if ( entry->value->alive ) {
                const char* taskIdentifier = entry->value->getIdentifier();
                if ( taskIdentifier != 0 && String::equals(taskIdentifier, identifier) ) {
                    thread = entry->value;
                    break;
                }
            }
            entry = entry->next;
        }
    }

    return thread;
}

/**
 * Calculate the number of tasks of provided type there are in the System
 *
 * @param type:		the type of task to count
 * @return the count of the task in this scheduler
 */
uint32_t Scheduler::count(ThreadType type) {
    uint32_t   count = 0;
    TaskEntry* entry = runQueue;
    while ( entry ) {
        if ( entry->value->type == type )
            ++count;
        entry = entry->next;
    }

    entry = waitQueue;
    while ( entry ) {
        if ( entry->value->type == type )
            ++count;
        entry = entry->next;
    }

    return count;
}

/**
 * Fills the out buffer whit
 * the thread ids of the tasks that this scheduler manage,
 * the buffer have a len length and return the index of the last copied Tid
 *
 * @param out:		the buffer of thread ids
 * @param len:		the length of the buffer
 * @param type:		the type of task id to store on buffer
 * @return the count of ids copied
 */
uint32_t Scheduler::getTaskIDs(Tid* out, uint32_t len, ThreadType type) {
    uint32_t   pos   = 0;
    TaskEntry* entry = runQueue;
    while ( pos < len && entry ) {
        if ( entry->value->type == type )
            out[pos++] = entry->value->id;
        entry = entry->next;
    }

    entry = waitQueue;
    while ( pos < len && entry ) {
        if ( entry->value->type == type )
            out[pos++] = entry->value->id;
        entry = entry->next;
    }

    return pos;
}

/**
 * Performs the actual context switch.
 *
 * @param thread:		the thread to switch to
 */
void Scheduler::_applyContextSwitch(Thread* thread) {
    // switch to the address space
    AddressSpace::switchToSpace(thread->process->pageDirectory);

    // set ESP0 in the TSS
    GdtManager::setTssEsp0(thread->kernelStackEsp0);
}

/**
 * Checks whether the given thread has died. Removes it (and all child threads).
 *
 * @param thread:		the thread to check
 * @return whether the thread as died
 */
bool Scheduler::_checkAliveState(Thread* thread) {
    // Eliminate if dead
    if ( !thread->alive ) {
        if ( thread->type == THREAD_TYPE_MAIN )
            Tasking::removeThreads(thread->process);
        else
            _remove(thread);

        return false;
    }

    return true;
}

/**
 * Removes the given thread from its queue.
 *
 * @param thread:		the thread to remove
 */
void Scheduler::_remove(Thread* thread) {
    // remove from run queue
    TaskEntry* entry = _removeFromQueue(&runQueue, thread);

    // if it was not in run queue, get it from wait queue
    if ( !entry )
        entry = _removeFromQueue(&waitQueue, thread);

    // if it was in neither queues, thats weird
    if ( !entry ) {
        logWarn("%! failed to properly delete thread %i, was not assigned to a queue",
                "scheduler",
                thread->id);
        return;
    }

    // delete entry and task
    delete entry;
    ThreadManager::deleteTask(thread);
}

/**
 * Finishs the context switch by setting userspace specific values.
 *
 * @param thread:		thread to finish the switch for
 */
void Scheduler::_finishSwitch(Thread* thread) {
    // write user thread address to GDT
    GdtManager::setUserThreadAddress(thread->userThreadAddr);

    // set GS of thread to user pointer segment
    thread->cpuState->gs = 0x30;
}

/**
 * Removes a thread from a queue.
 *
 * @param queue_head:		pointer to a queues head
 * @param thread:			the thread to remove
 * @return the thread entry or nullptr if not found
 */
TaskEntry* Scheduler::_removeFromQueue(TaskEntry** queueHead, Thread* thread) {
    TaskEntry* removedEntry = 0;
    TaskEntry* entry        = *queueHead;

    // if queue is empty, entry can't be removed
    if ( !entry )
        return 0;

    // if it's the head of the queue, replace it
    if ( entry->value == thread ) {
        removedEntry = entry;
        *queueHead   = removedEntry->next;
    }

    else {
        // otherwise, find entry before it and replace it
        TaskEntry* previous = 0;
        while ( entry ) {
            if ( entry->value == thread ) {
                removedEntry   = entry;
                previous->next = removedEntry->next;
                break;
            }
            previous = entry;
            entry    = entry->next;
        }
    }

    return removedEntry;
}

/**
 * Processes all threads in the wait queue. Checks the waiting state for each
 * thread in the wait queue.
 */
void Scheduler::_processWaitQueue() {
    // process wait queue
    currentEntry = waitQueue;

    while ( currentEntry ) {
        TaskEntry* next = currentEntry->next;

        // switch to tasks space
        _applyContextSwitch(currentEntry->value);

        // remove it if its dead and check its waiting state
        if ( _checkAliveState(currentEntry->value) )
            _checkWaitingState(currentEntry->value);

        currentEntry = next;
    }
}

/**
 * Checks the thread for any waiting operations, performs these and moves the
 * thread to the run queue if possible.
 *
 * @param thread:		the thread to check
 */
void Scheduler::_checkWaitingState(Thread* thread) {
    // check if task must continue waiting
    if ( thread->waitManager ) {
        if (String::equals(thread->getIdentifier(),"ZipNET")) {
            logInfo("ZipNET Waits for %s", thread->waitManager->debugName());
        }

        if ( thread->checkWaiting() ) {
            // increase wait counter for deadlock warnings
            thread->waitCount++;
            if ( !(thread->waitCount % 500000) )
                _printDeadlockWarning();
        } else {
            // reset wait counter & remove wait handler
            thread->waitCount = 0;
            thread->unwait();
        }
    }
}

/**
 * Prints a deadlock warning for the current thread.
 */
void Scheduler::_printDeadlockWarning() {
    char* taskName = (char*)"?";
    if ( currentEntry->value->getIdentifier() != 0 )
        taskName = (char*)currentEntry->value->getIdentifier();

    logDebug("%! thread %i (process %i, named '%s') waits for '%s'",
             "deadlock-detector",
             currentEntry->value->id,
             currentEntry->value->process->main->id,
             taskName,
             currentEntry->value->waitManager->debugName());
}
