/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include <debug/DebugInterfaceKernel.hpp>
#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <system/ProcessorState.hpp>
#include <system/system.hpp>
#include <tasking/scheduling/scheduler.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>

/**
 * cpu task schedulers instances
 */
Scheduler** Tasking::schedulers;

/**
 * registered kernel Servers
 */
llist<Process*>* Tasking::servers;

/**
 * initialize the interface creating
 * the space for schedulers of each cores
 */
void Tasking::initialize() {
    // create space for all schedulers
    uint32_t numCores = System::getNumberOfProcessors();
    schedulers        = new Scheduler*[numCores];

    // nullificate all pointers
    for ( uint32_t i = 0; i < numCores; i++ )
        schedulers[i] = 0;

    // initializate the server list
    servers = new llist<Process*>();
}

/**
 * create the scheduler for the core that call this function
 */
void Tasking::enableForThisCore() {
    // create a new scheduler for this core
    uint32_t coreID    = System::currentProcessorId();
    schedulers[coreID] = new Scheduler(coreID);

    logInfo("%! scheduler installed on core %i", "tasking", coreID);
}

/**
 * creates a new thread forking from the provided,
 * it must be a main thread of a process
 *
 * @param currentThread:	the thread to be forked
 * @return the new forked thread
 */
Thread* Tasking::fork(Thread* currentThread) {
    Thread* clone = 0;

    // only main thread can be forked
    if ( currentThread == currentThread->process->main ) {
        if ( currentThread ) {
            clone = ThreadManager::fork(currentThread);
            if ( clone )
                addTask(clone);
        }
    }

    else
        logInfo("%! tryed to fork a non main thread", "tasking");

    return clone;
}

/**
 * adds a new task on least loading core (only in AP)
 *
 * @param task:						the new task to be added
 * @param-opt enforceCurrentCore:	force the loading to the current
 * scheduler
 */
void Tasking::addTask(Thread* task, bool enforceCurrentCore) {
    Scheduler* target = 0;

    // Used by AP's for the idle binary
    if ( enforceCurrentCore )
        target = currentScheduler();

    else {
        // Find core with lowest load
        Scheduler* lowest     = 0;
        uint32_t   lowestLoad = 0;
        uint32_t   processors = System::getNumberOfProcessors();

        for ( uint32_t i = 0; i < processors; i++ ) {
            if ( schedulers[i] ) {
                // Check if load is lower than others
                uint32_t load = schedulers[i]->calculateLoad();
                if ( !lowest || load < lowestLoad ) {
                    lowest     = schedulers[i];
                    lowestLoad = load;
                }
            }
        }

        target = lowest;
    }

    // Error check
    if ( !target )
        EvaKernel::panic("%! couldn't find scheduler to add task to", "tasking");

    // Assign task to scheduler
    target->add(task);
}

/**
 * saves the provided cpu state of the current task on
 * the current scheduler and return the thread instance where state is been
 * saved
 *
 * @param cpuState:		the cpu state to save
 * @return the thread object where is been saves the state
 */
Thread* Tasking::save(ProcessorState* cpuState) {
    return currentScheduler()->save(cpuState);
}

/**
 * call scheduler to choose the next task to be executed, return the next task
 *
 * @return the new task to execute
 */
Thread* Tasking::schedule() {
    return currentScheduler()->schedule();
}

/**
 * pushes the given thread to the top of the wait queue
 *
 * @param task:		the task to increase to
 */
void Tasking::increaseWaitPriority(Thread* thread) {
    currentScheduler()->increaseWaitPriority(thread);
}

/**
 * @returns the current task on the current core
 */
Thread* Tasking::lastThread() {
    return currentScheduler()->lastThread();
}

/**
 * @returns the current scheduler on the current core
 */
Scheduler* Tasking::currentScheduler() {
    uint32_t   coreID    = System::currentProcessorId();
    Scheduler* scheduler = schedulers[coreID];

    // Error check
    if ( !scheduler )
        EvaKernel::panic("%! no scheduler exists for core %i", "tasking", coreID);

    return scheduler;
}

/**
 * kills all threads of the given process within all schedulers
 * returns true if all are dead
 *
 * @param process:		the process of which the threads shall be killed
 */
void Tasking::removeThreads(Process* process) {
    // remove from the server list if is a server
    if ( process->isServer )
        servers->erase(process);

    // remove other threads
    uint32_t processors = System::getNumberOfProcessors();
    for ( uint32_t i = 0; i < processors; i++ ) {
        Scheduler* scheduler = schedulers[i];
        if ( scheduler )
            scheduler->removeThreads(process);
    }
}

/**
 * Adds the provided process to the servers list and
 * register as new server if it doesn't already exists
 *
 * @param process:		the process to registrate
 * @return whether the operation success
 */
uint8_t Tasking::addServer(Process* process) {
    logDebug("%! Trying to registering a process as Server", "tasking");

    // don't re-register the process
    if ( !process->isServer ) {
        // find if there is another server with the same name
        for ( llist<Process*>::iterator it = servers->begin(); it != servers->end(); ++it )
            if ( StringUtils::equals(process->main->getIdentifier(), (*it)->main->getIdentifier()) )
                return false;

        // add to the list
        process->isServer = true;
        servers->add(process);
        logDebug("%! Now '%s' is a server", "tasking", process->main->getIdentifier());
    }
    return true;
}

/**
 * Returns the process descriptor of the server with the name provided
 *
 * @param identifier:		the identifier of the server
 * @return the process descriptor or 0
 */
Process* Tasking::getServer(const char* identifier) {
    // find if there is another server with the same name
    for ( llist<Process*>::iterator it = servers->begin(); it != servers->end(); ++it )
        if ( StringUtils::equals(identifier, (*it)->main->getIdentifier()) )
            return *it;

    return 0;
}

/**
 * @return the thread descriptor from it's Tid
 */
Thread* Tasking::getTaskById(uint32_t id) {
    // parse each scheduler
    uint32_t processors = System::getNumberOfProcessors();
    for ( uint32_t i = 0; i < processors; i++ ) {
        // check always scheduler validity
        if ( schedulers[i] ) {
            Thread* task = schedulers[i]->getTaskById(id);
            if ( task )
                return task;
        }
    }

    return 0;
}

/**
 * @return the thread descriptor from it's name
 */
Thread* Tasking::getTaskByIdentifier(const char* identifier) {
    // parse each scheduler
    uint32_t processors = System::getNumberOfProcessors();
    for ( uint32_t i = 0; i < processors; i++ ) {
        // check always scheduler validity
        if ( schedulers[i] ) {
            Thread* task = schedulers[i]->getTaskByIdentifier(identifier);
            if ( task )
                return task;
        }
    }

    return 0;
}

/**
 * concat a numeric id to the task identifier if task id is already in use
 *
 * @param target:					the target thread to
 * adjust
 * @param firstExist:				the first existing thread with same
 * name that we shall set
 * @param preferredIdentifier:		identifier to adjust and set to target
 * @return the setupped identifier for the target
 */
const char*
Tasking::adjustTaskIdentifier(Thread* target, Thread* firstExist, const char* preferredIdentifier) {
    // legalize the identifier from illegal characters
    StringUtils::replace((char*)preferredIdentifier, ':', '_');

    // find all task with this name and add to new
    int     lastNum = 1;
    Thread* next    = firstExist;

    // iterate while find a valid task
    while ( next != 0 ) {
        // create the identifier
        const char* strnum = StringUtils::fromInt(lastNum, BASE_DECIMAL);
        char        nextID[StringUtils::length(preferredIdentifier) + StringUtils::length(strnum) + 1];
        StringUtils::concat(preferredIdentifier, strnum, nextID);

        // remove the string and increase the num
        delete strnum;
        lastNum++;

        // get from schedulers
        next = getTaskByIdentifier(nextID);
    }

    // now create the definitive indentifier
    const char* effectivenum = StringUtils::fromInt(lastNum, BASE_DECIMAL);
    char        identifier[StringUtils::length(preferredIdentifier) + StringUtils::length(effectivenum) + 1];
    StringUtils::concat(preferredIdentifier, effectivenum, identifier);

    // set the identifier
    target->setIdentifier(identifier);

    // return the setupped identifier
    return target->getIdentifier();
}

/**
 * register a new name for the provided task
 *
 * @param task:				the task that want a new name
 * @param newIdentifier:	the new identifier to be setted
 */
bool Tasking::registerTaskForIdentifier(Thread*     task,
                                        const char* newIdentifier,
                                        char*       selectedIdentifier) {
    // first take the complete name
    /*char completeName[STRING_IDENTIFIER_MAX_LENGTH];

    // check thread type
    if (task->type == THREAD_TYPE_SUB)
    {
            // get a duplicate of the name
            char *previd = StringUtils::duplicate(task->getIdentifier());
            uint32_t length = StringUtils::length(previd);

            // the thread name is already appended
            if (previd[length - 1] != ':')
            {
                    // remove the thread name appendix
                    uint32_t lastDoubleDot = StringUtils::lastIndexOf(previd, ':');
                    uint32_t appendixLen = length - lastDoubleDot - 1;
                    Memory::setWords(&previd[lastDoubleDot], '\0', appendixLen);
            }

            // no name appended to root, concat and destroy duplicate
            StringUtils::concat(previd, newIdentifier, completeName);
            delete previd;
    }

    // thread is a main or vm86
    else StringUtils::copy(completeName, task->getIdentifier());

    // test name
    if (*completeName != '\0')
    {
            // Check if someone else has this identifier
            Thread *existing = getTaskByIdentifier(newIdentifier);
            if (existing && existing != task)
            {
                    // show warning
                    logWarn("%! task %i could not be registered as '%s', auto
    adding a supplementary numeric id", "tasking", task->id, newIdentifier);

                    // we have to adjust the name
                    if (selectedIdentifier) StringUtils::copy(selectedIdentifier,
    adjustTaskIdentifier(task, existing, newIdentifier)); return false;
            }
    }*/

    // Check if someone else has this identifier
    Thread* existing = getTaskByIdentifier(newIdentifier);
    if ( existing ) {
        logWarn("%! task %i could not be registered as '%s', name is used by %i",
                "tasking",
                task->id,
                newIdentifier,
                existing->id);
        return false;
    }

    // Set the identifier
    task->setIdentifier(newIdentifier);

    DEBUG_INTERFACE_TASK_SET_IDENTIFIER(task->id, newIdentifier);
    logDebug("%! task %i registered as '%s'", "tasking", task->id, newIdentifier);
    return true;
}

/**
 * Calculate the number of tasks of provided type there are in the System
 *
 * @param type:		the type of task to count
 * @return the count of the task in all schedulers
 */
uint32_t Tasking::count(ThreadType type) {
    uint32_t total      = 0;
    uint32_t processors = System::getNumberOfProcessors();

    // test first all schedulers
    for ( uint32_t i = 0; i < processors; i++ )
        if ( !schedulers[i] )
            return total;

    // shift to check main type
    if ( type & THREAD_TYPE_MAIN )
        for ( uint32_t i = 0; i < processors; i++ )
            total += schedulers[i]->count(THREAD_TYPE_MAIN);

    // shift to check vm86 type
    if ( type & THREAD_TYPE_VM86 )
        for ( uint32_t i = 0; i < processors; i++ )
            total += schedulers[i]->count(THREAD_TYPE_VM86);

    // shift to check sub type
    if ( type & THREAD_TYPE_SUB )
        for ( uint32_t i = 0; i < processors; i++ )
            total += schedulers[i]->count(THREAD_TYPE_SUB);

    return total;
}

/**
 * fill the out buffer with the id's of the tasks
 *
 * @param out:		the buffer where store the ids
 * @param len:		the size of the buffer
 * @param type:		the type of task id to store on buffer
 * @return the count of copied ids
 */
uint32_t Tasking::getTaskIDs(Tid* out, uint32_t len, ThreadType type) {
    uint32_t pos        = 0;
    uint32_t processors = System::getNumberOfProcessors();

    // test first all schedulers
    for ( uint32_t i = 0; i < processors; i++ )
        if ( !schedulers[i] )
            return pos;

    // shift to check main type
    if ( type & THREAD_TYPE_MAIN )
        for ( uint32_t i = 0; i < processors; i++ )
            pos += schedulers[i]->getTaskIDs(&out[pos], len - pos, THREAD_TYPE_MAIN);

    // shift to check vm86 type
    if ( type & THREAD_TYPE_VM86 )
        for ( uint32_t i = 0; i < processors; i++ )
            pos += schedulers[i]->getTaskIDs(&out[pos], len - pos, THREAD_TYPE_VM86);

    // shift to check sub type
    if ( type & THREAD_TYPE_SUB )
        for ( uint32_t i = 0; i < processors; i++ )
            pos += schedulers[i]->getTaskIDs(&out[pos], len - pos, THREAD_TYPE_SUB);

    return pos;
}
