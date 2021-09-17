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

#include <calls/SyscallHandler.hpp>
#include <EvangelionNG.hpp>
#include <filesystem/filesystem.hpp>
#include <logger/logger.hpp>
#include <system/interrupts/handling/InterruptRequestHandler.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/wait/WaiterAtomicWait.hpp>
#include <tasking/wait/WaiterJoin.hpp>
#include <tasking/wait/WaiterSleep.hpp>
#include <tasking/wait/WaiterWaitForIrq.hpp>

/**
 * Join to another thread and let the current
 * thread to sleep with a join waiter to wait the joined task's termination
 */
SYSCALL_HANDLER(join) {
    SyscallJoin* data = (SyscallJoin*)SYSCALL_DATA(currentThread->cpuState);
    currentThread->wait(new WaiterJoin(data->taskID));
    return Tasking::schedule();
}

/**
 * call scheduler and switch to next task
 */
SYSCALL_HANDLER(yield) {
    return Tasking::schedule();
}

/**
 * Returns the id of the current thread.
 */
SYSCALL_HANDLER(getTid) {
    SyscallGetTid* data = (SyscallGetTid*)SYSCALL_DATA(currentThread->cpuState);
    data->id            = currentThread->id;
    return currentThread;
}

/**
 * Returns the id of the current process. If the executing task is a thread, the id of the
 * root process is returned.
 */
SYSCALL_HANDLER(getPid) {
    SyscallGetPid* data = (SyscallGetPid*)SYSCALL_DATA(currentThread->cpuState);
    data->id            = currentThread->process->main->id;
    return currentThread;
}

/**
 * Return the number of milliseconds that the current scheduler runs
 */
SYSCALL_HANDLER(millis) {
    SyscallMillis* data = (SyscallMillis*)SYSCALL_DATA(currentThread->cpuState);
    data->millis        = Tasking::currentScheduler()->getMilliseconds();
    return currentThread;
}

/**
 * Returns the name of the Thread provided by Tid
 */
SYSCALL_HANDLER(getThreadName) {
    SyscallGetIdentifier* data   = (SyscallGetIdentifier*)SYSCALL_CODE(currentThread->cpuState);
    Thread*               target = Tasking::getTaskById(data->id);

    // check existance
    if ( target ) {
        const char* identifier = target->getIdentifier();
        if ( identifier )
            String::copy(data->identifier, identifier);
        else
            data->identifier[0] = '\0';
    }

    // set a terminated string
    else
        data->identifier[0] = '\0';

    return currentThread;
}

/**
 * Returns the process id for a task id.
 */
SYSCALL_HANDLER(getPidForTid) {
    SyscallGetPidForTid* data       = (SyscallGetPidForTid*)SYSCALL_DATA(currentThread->cpuState);
    Thread*              targetTask = Tasking::getTaskById(data->tid);
    data->pid                       = targetTask->process->main->id;

    return currentThread;
}

/**
 * Returns the parent process id for the given process id.
 */
SYSCALL_HANDLER(getParentPid) {
    SyscallGetParentPid* data       = (SyscallGetParentPid*)SYSCALL_DATA(currentThread->cpuState);
    Thread*              targetTask = Tasking::getTaskById(data->pid);

    Process* parentProcess = targetTask->process->parent;
    if ( parentProcess )
        data->parentPid = parentProcess->main->id;
    else
        data->parentPid = -1;

    return currentThread;
}

/**
 * return the Thread ID of the Thread from the provided string name
 */
SYSCALL_HANDLER(getTidByName) {
    SyscallTaskIDGet* data = (SyscallTaskIDGet*)SYSCALL_DATA(currentThread->cpuState);

    Thread* resultTask = Tasking::getTaskByIdentifier((const char*)data->identifier);
    if ( resultTask != 0 )
        data->resultTaskID = resultTask->id;
    else
        data->resultTaskID = -1;

    return currentThread;
}

/**
 * Register a new name for the current Thread
 */
SYSCALL_HANDLER(registerThreadName) {
    SyscallTaskIDRegister* data = (SyscallTaskIDRegister*)SYSCALL_DATA(currentThread->cpuState);
    data->success
        = Tasking::registerTaskForIdentifier(currentThread, data->identifier, data->selectedID);
    return currentThread;
}

/**
 * Kills a task
 */
SYSCALL_HANDLER(kill) {
    SyscallKill* data   = (SyscallKill*)SYSCALL_DATA(currentThread->cpuState);
    Thread*      target = Tasking::getTaskById(data->pid);

    // if thread doesn't exist
    if ( !target )
        data->status = KILL_STATUS_NOT_FOUND;

    // if thread is main kill entire process
    else if ( target->type == THREAD_TYPE_MAIN ) {
        target->process->main->alive = false;
        data->status                 = KILL_STATUS_SUCCESSFUL;
    }

    // else kill only current thread
    else {
        target->alive = false;
        data->status  = KILL_STATUS_SUCCESSFUL;
    }

    // schedule, clear lists
    return Tasking::schedule();
}

/**
 * Sets the status of the current task to dead and yields. On the next call
 * to the scheduler this process is removed completely.
 */
SYSCALL_HANDLER(exit) {
    currentThread->process->main->alive = false;
    return Tasking::schedule();
}

/**
 * Exits only the current thread.
 */
SYSCALL_HANDLER(exitThread) {
    currentThread->alive = false;
    return Tasking::schedule();
}

/**
 * Lets the current thread sleep for the specified number of milliseconds. If the caller
 * specifies a negative number, the process doesn't sleep and returns normally.
 */
SYSCALL_HANDLER(sleep) {
    SyscallSleep* data = (SyscallSleep*)SYSCALL_DATA(currentThread->cpuState);
    if ( data->milliseconds > 0 )
        currentThread->wait(new WaiterSleep(currentThread, data->milliseconds));
    return Tasking::schedule();
}

/**
 * performs an atomic sincronization with atoms betweens two threads
 */
SYSCALL_HANDLER(atomicWait) {
    SyscallAtomicLock* data = (SyscallAtomicLock*)SYSCALL_DATA(currentThread->cpuState);

    // when "trying" only...
    if ( data->tryOnly ) {
        // check if atom 1 is set and atom 2 is NULL or set
        if ( *data->atom1 && (!data->atom2 || *data->atom2) )
            data->wasSet = false;
        else {
            *data->atom1 = true;
            if ( data->atom2 )
                *data->atom2 = true;
            data->wasSet = true;
        }
        return currentThread;
    }

    // check if atom 1 is set and atom 2 is NULL or set
    if ( *data->atom1 && (!data->atom2 || *data->atom2) ) {
        currentThread->wait(new WaiterAtomicWait(data->atom1, data->atom2, data->setOnFinish));
        return Tasking::schedule();
    }

    // already unlocked, set atoms
    if ( data->setOnFinish ) {
        *data->atom1 = true;

        if ( data->atom2 )
            *data->atom2 = true;
    }
    return currentThread;
}

/**
 * The interrupt polling mechanism allows programs to wait until an interrupt is
 * fired. If the interrupt was already fired, this call immediately returns. Otherwise,
 * the task waits until the interrupt happens.
 */
SYSCALL_HANDLER(waitForIrq) {
    SyscallWaitForIrq* data = (SyscallWaitForIrq*)SYSCALL_DATA(currentThread->cpuState);

    // Only driver level
    if ( currentThread->process->securityLevel == SECURITY_LEVEL_DRIVER ) {
        bool fired = InterruptRequestHandler::pollIrq(data->irq);
        if ( fired )
            return currentThread;
        else {
            currentThread->wait(new WaiterWaitForIrq(data->irq));
            return Tasking::schedule();
        }
    }

    return currentThread;
}

/**
 * performs a fork from the currentThread, the currentThread must be a main thread or a vm86
 */
SYSCALL_HANDLER(fork) {
    SyscallFork* data   = (SyscallFork*)SYSCALL_DATA(currentThread->cpuState);
    Thread*      forked = Tasking::fork(currentThread);

    if ( forked ) {
        // clone file descriptors
        FileSystem::processForked(currentThread->process->main->id, forked->process->main->id);

        // return forked id in target process
        data->forkedID = forked->id;

        // switch to clone for a moment, set return value to 0
        PageDirectory cur = AddressSpace::getCurrentSpace();
        AddressSpace::switchToSpace(forked->process->pageDirectory);
        data->forkedID = 0;
        AddressSpace::switchToSpace(cur);
    }

    else
        data->forkedID = -1;

    return currentThread;
}

/**
 * Lest the current thread to register an userspace handler (usually a function) as an IRQ handler
 */
SYSCALL_HANDLER(registerIrqHandler) {
    SyscallRegisterIrqHandler* data
        = (SyscallRegisterIrqHandler*)SYSCALL_DATA(currentThread->cpuState);

    if ( currentThread->process->securityLevel <= SECURITY_LEVEL_DRIVER ) {
        InterruptRequestHandler::setHandler(data->irq,
                                            currentThread->id,
                                            data->handler,
                                            data->callback);
        data->status = REGISTER_IRQ_HANDLER_STATUS_SUCCESSFUL;
    }

    else
        data->status = REGISTER_IRQ_HANDLER_STATUS_NOT_PERMITTED;

    return currentThread;
}

/**
 * restore the state of the thread from a previous interruption
 */
SYSCALL_HANDLER(restoreInterruptedState) {
    currentThread->restoreInterruptedState();
    return Tasking::schedule();
}

/**
 * Lest the current thread to register an userspace handler (usually a function) as a signal handler
 */
SYSCALL_HANDLER(registerSignalHandler) {
    SyscallRegisterSignalHandler* data
        = (SyscallRegisterSignalHandler*)SYSCALL_DATA(currentThread->cpuState);

    if ( data->signal >= 0 && data->signal < SIG_COUNT ) {
        SignalHandler* handler = &(currentThread->process->signalHandlers[data->signal]);

        data->previousHandler = handler->handler;
        handler->handler      = data->handler;
        handler->callback     = data->callback;
        handler->threadID     = currentThread->id;
        data->status          = REGISTER_SIGNAL_HANDLER_STATUS_SUCCESSFUL;

        logDebug("%! signal handler %h registered for signal %i",
                 "syscall",
                 data->handler,
                 data->signal);
    }

    else {
        data->previousHandler = -1;
        data->status          = REGISTER_SIGNAL_HANDLER_STATUS_INVALID_SIGNAL;
        logDebug("%! failed to register signal handler %h for invalid signal %i",
                 "syscall",
                 data->handler,
                 data->signal);
    }

    return currentThread;
}

/**
 * Sends the provided signal to the target thread
 */
SYSCALL_HANDLER(raiseSignal) {
    SyscallRaiseSignal* data = (SyscallRaiseSignal*)SYSCALL_DATA(currentThread->cpuState);

    if ( data->signal >= 0 && data->signal < SIG_COUNT ) {
        // get main thread by id
        Thread* targetThread = 0;

        if ( currentThread->id == data->process )
            targetThread = currentThread;
        else if ( currentThread->process->main->id == data->process )
            targetThread = currentThread->process->main;
        else
            targetThread = Tasking::getTaskById(data->process);

        // target process doesn't exist
        if ( !targetThread )
            data->status = RAISE_SIGNAL_STATUS_INVALID_TARGET;

        else {
            // raise the signal
            targetThread->raiseSignal(data->signal);
            data->status = RAISE_SIGNAL_STATUS_SUCCESSFUL;
        }

    }

    // signal doesn't exist
    else
        data->status = RAISE_SIGNAL_STATUS_INVALID_SIGNAL;

    return Tasking::schedule();
}
