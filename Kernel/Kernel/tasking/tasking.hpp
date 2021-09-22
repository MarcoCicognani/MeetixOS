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

#ifndef EVA_TASKING_TASKING
#define EVA_TASKING_TASKING

#include "Api/Kernel.h"
#include "Api/StdInt.h"

#include <Api/utils/llist.hpp>
#include <system/ProcessorState.hpp>
#include <tasking/process.hpp>
#include <tasking/scheduling/scheduler.hpp>
#include <tasking/thread.hpp>

/**
 * high level interface to manage task and schedulers on multiple cpus or on single cpu
 */
class Tasking {
private:
    /**
     * list of schedulers for each cpus
     */
    static Scheduler** schedulers;

    /**
     * list that contains the pointer to the registered server processes
     */
    static llist<Process*>* servers;

public:
    /**
     * initialize the interface creating
     * the space for schedulers of each cores
     */
    static void initialize();

    /**
     * create the scheduler for the core that call this function
     */
    static void enableForThisCore();

    /**
     * creates a new thread forking from the provided,
     * it must be a main thread of a process
     *
     * @param currentThread:	the thread to be forked
     * @return the new forked thread
     */
    static Thread* fork(Thread* currentThread);

    /**
     * adds a new task on least loading core (only in AP)
     *
     * @param task:						the new task to be added
     * @param-opt enforceCurrentCore:	force the loading to the current scheduler
     */
    static void addTask(Thread* task, bool enforceCurrentCore = false);

    /**
     * saves the provided cpu state of the current task on
     * the current scheduler and return the thread instance where state is been saved
     *
     * @param cpuState:		the cpu state to save
     * @return the thread object where is been saves the state
     */
    static Thread* save(ProcessorState* cpuState);

    /**
     * call scheduler to choose the next task to be executed, return the next task
     *
     * @return the new task to execute
     */
    static Thread* schedule();

    /**
     * pushes the given thread to the top of the wait queue
     *
     * @param task:		the task to increase to
     */
    static void increaseWaitPriority(Thread* task);

    /**
     * @returns the current task on the current core
     */
    static Thread* lastThread();

    /**
     * @returns the current scheduler on the current core
     */
    static Scheduler* currentScheduler();

    /**
     * kills all threads of the given process within all schedulers
     * returns true if all are dead
     *
     * @param process:		the process of which the threads shall be killed
     */
    static void removeThreads(Process* process);

    /**
     * Adds the provided process to the servers list and
     * register as new server if it doesn't already exists
     *
     * @param process:		the process to registrate
     * @return whether the operation success
     */
    static uint8_t addServer(Process* process);

    /**
     * Returns the process descriptor of the server with the name provided
     *
     * @param identifier:		the identifier of the server
     * @return the process descriptor or 0
     */
    static Process* getServer(const char* identifier);

    /**
     * @return the thread descriptor from it's Tid
     */
    static Thread* getTaskById(uint32_t id);

    /**
     * @return the thread descriptor from it's name
     */
    static Thread* getTaskByIdentifier(const char* identifier);

    /**
     * concat a numeric id to the task identifier if task id is already in use
     *
     * @param target:					the target thread to adjust
     * @param firstExist:				the first existing thread with same name that
     * we shall set
     * @param preferredIdentifier:		identifier to adjust and set to target
     * @return the setupped identifier for the target
     */
    static const char*
    adjustTaskIdentifier(Thread* target, Thread* firstExist, const char* preferredIdentifier);

    /**
     * register a new name for the provided task
     *
     * @param task:				the task that want a new name
     * @param newIdentifier:	the new identifier to be setted
     */
    static bool
    registerTaskForIdentifier(Thread* task, const char* newIdentifier, char* selectedIdentifier);

    /**
     * Calculate the number of tasks of provided type there are in the System
     *
     * @param type:		the type of task to count
     * @return the count of the task in all schedulers
     */
    static uint32_t count(ThreadType type);

    /**
     * fill the out buffer with the id's of the tasks
     *
     * @param out:		the buffer where store the ids
     * @param len:		the size of the buffer
     * @param type:		the type of task id to store on buffer
     * @return the count of copied ids
     */
    static uint32_t getTaskIDs(Tid* out, uint32_t len, ThreadType type);
};

#endif
