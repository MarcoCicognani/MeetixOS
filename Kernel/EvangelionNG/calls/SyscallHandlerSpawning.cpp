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
#include "eva/kernel.h"
#include "eva/utils/local.hpp"

#include <calls/SyscallHandler.hpp>
#include <EvangelionNG.hpp>
#include <executable/Elf32Loader.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/physical/PPreferenceTracker.hpp>
#include <ramdisk/ramdisk.hpp>
#include <tasking/tasking.hpp>
#include <tasking/thread.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/wait/WaiterCallVm86.hpp>

/**
 * Maximim pages allocable for each call
 */
#define CREATE_PAGE_IN_SPACE_MAXIMUM_PAGES 100

/**
 * Allocate a new empty Process descriptor on the kernel heap
 */
SYSCALL_HANDLER(createEmptyProcess) {
    // Prepare data
    SyscallCreateEmptyProcess* data
        = (SyscallCreateEmptyProcess*)SYSCALL_DATA(currentThread->cpuState);
    data->processObject = 0;

    Process* parentProcess = currentThread->process;

    // Only kernel level
    if ( parentProcess->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread* emptyMainThread = ThreadManager::createProcess(data->securityLevel, parentProcess);
        data->processObject     = emptyMainThread;

        logDebug("%! (%i:%i) created empty process %i",
                 "syscall",
                 parentProcess->main->id,
                 currentThread->id,
                 emptyMainThread->id);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: create empty process",
                "syscall",
                parentProcess->main->id,
                currentThread->id);
    }

    return currentThread;
}

/**
 * Copy on the cli buffer of the process under construction the arguments
 */
SYSCALL_HANDLER(cliArgsStore) {
    SyscallCliArgsStore* data = (SyscallCliArgsStore*)SYSCALL_DATA(currentThread->cpuState);

    // only kernel level tasks can do this
    if ( currentThread->process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        // get the objects
        Thread*  other        = (Thread*)data->processObject;
        Process* otherProcess = other->process;

        // allocate the command line buffer
        otherProcess->cliArguments = new char[CLIARGS_BUFFER_LENGTH];
        uint32_t argsLen           = String::length(data->arguments);
        Memory::copy(otherProcess->cliArguments, data->arguments, argsLen);
        otherProcess->cliArguments[argsLen] = 0;

        logDebug("%! task %i stored cli arguments for task %i",
                 "syscall",
                 currentThread->id,
                 other->id);
    }

    else {
        logWarn("%! task %i tried to store another tasks cli arguments but is not permitted",
                "syscall",
                currentThread->id);
    }

    return currentThread;
}

/**
 * Destroy the cli buffer of the process and returns to the currentThread the previous provided
 * arguments
 */
SYSCALL_HANDLER(cliArgsRelease) {
    Process* process = currentThread->process;

    SyscallCliArgsRelease* data = (SyscallCliArgsRelease*)SYSCALL_DATA(currentThread->cpuState);

    // Copy args if available
    if ( process->cliArguments != 0 ) {
        Memory::copy(data->buffer, process->cliArguments, CLIARGS_BUFFER_LENGTH);
        data->buffer[String::length(process->cliArguments)] = 0;

        delete process->cliArguments;
        process->cliArguments = 0;
    }

    // Null-terminate in case of failure
    else
        data->buffer[0] = 0;

    return currentThread;
}

/**
 * Allocate memory pages of the kernel for the process that is loading
 */
SYSCALL_HANDLER(createPagesInSpace) {
    Process* process = currentThread->process;

    // Prepare data
    SyscallCreatePagesInSpace* data
        = (SyscallCreatePagesInSpace*)SYSCALL_DATA(currentThread->cpuState);
    data->resultVirtualAddress = 0;

    // Only kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread*  targetThread  = (Thread*)data->processObject;
        Process* targetProcess = targetThread->process;

        // Create page in target space
        // Below we do a temporary switch, where 'data' is not available (because we switch to the
        // target userspace, and 'data' is in the current userspace), so we copy the values:
        uint32_t virtualAddressInTargetSpace = data->targetSpaceVirtualAddress;
        uint32_t numberOfPages               = data->numberOfPages;

        // Only allow a certain number of pages to be cross-allocated
        if ( numberOfPages > 0 && numberOfPages <= CREATE_PAGE_IN_SPACE_MAXIMUM_PAGES ) {
            // Adjust the image range of the other process if necessary. This is required so that
            // the kernel can keep track of where the process image lays in this address space.
            if ( !targetProcess->imageStart
                 || targetProcess->imageStart > virtualAddressInTargetSpace )
                targetProcess->imageStart = virtualAddressInTargetSpace;
            if ( !targetProcess->imageEnd
                 || targetProcess->imageEnd
                        < (virtualAddressInTargetSpace + numberOfPages * PAGE_SIZE) )
                targetProcess->imageEnd = virtualAddressInTargetSpace + numberOfPages * PAGE_SIZE;

            // Create physical pages and map them into the target space. Remember the physical
            // addresses
            PhysicalAddress physicalPages[numberOfPages];

            // Perform temporary switch to target process and map pages
            AddressSpace::switchToSpace(targetProcess->pageDirectory);
            for ( uint32_t i = 0; i < numberOfPages; i++ ) {
                physicalPages[i] = PPallocator::allocate();
                AddressSpace::map(virtualAddressInTargetSpace + i * PAGE_SIZE,
                                  physicalPages[i],
                                  DEFAULT_USER_TABLE_FLAGS,
                                  DEFAULT_USER_PAGE_FLAGS);
                PPreferenceTracker::increment(physicalPages[i]);
            }
            AddressSpace::switchToSpace(process->pageDirectory);

            // Map all pages (which physical addresses are in the array) to the current tasks space
            VirtualAddress virtAddrHere
                = process->virtualRanges.allocate(numberOfPages, PROC_VIRTUAL_RANGE_FLAG_NONE);
            for ( uint32_t i = 0; i < numberOfPages; i++ )
                AddressSpace::map(virtAddrHere + i * PAGE_SIZE,
                                  physicalPages[i],
                                  DEFAULT_USER_TABLE_FLAGS,
                                  DEFAULT_USER_PAGE_FLAGS);

            data->resultVirtualAddress = virtAddrHere;

            logDebug(
                "%! (%i:%i) created %i pages in space of process %i. virtual: here %h, there %h",
                "syscall",
                process->main->id,
                currentThread->id,
                data->numberOfPages,
                targetThread->id,
                virtAddrHere,
                data->targetSpaceVirtualAddress);
        }

        else {
            logWarn("%! (%i:%i) error: illegal arguments: tried to create pages in a space. %i is "
                    "not a valid number of pages (1-%i)",
                    "syscall",
                    process->main->id,
                    currentThread->id,
                    data->numberOfPages,
                    CREATE_PAGE_IN_SPACE_MAXIMUM_PAGES);
        }
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: create a page in a space",
                "syscall",
                process->main->id,
                currentThread->id);
    }

    return currentThread;
}

/**
 * Add the pending task to the execution queue
 */
SYSCALL_HANDLER(attachCreatedProcess) {
    SyscallAttachCreatedProcess* data
        = (SyscallAttachCreatedProcess*)SYSCALL_DATA(currentThread->cpuState);

    // Only kernel level
    if ( currentThread->process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread* targetMainThread        = (Thread*)data->processObject;
        targetMainThread->cpuState->eip = data->eip;
        ThreadManager::prepareThreadLocalStorage(targetMainThread);
        Tasking::addTask(targetMainThread);

        logDebug("%! (%i:%i) attached task %i, starting at eip %h",
                 "syscall",
                 currentThread->process->main->id,
                 currentThread->id,
                 targetMainThread->id,
                 targetMainThread->cpuState->eip);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: attach created process",
                "syscall",
                currentThread->process->main->id,
                currentThread->id);
    }

    return currentThread;
}

/**
 * Something is gone wrong, the creation process has encountered an error, we have to destroy the
 * task under construction
 */
SYSCALL_HANDLER(cancelProcessCreation) {
    SyscallCancelProcessCreation* data
        = (SyscallCancelProcessCreation*)SYSCALL_DATA(currentThread->cpuState);

    // Only kernel level
    if ( currentThread->process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread* target = (Thread*)data->processObject;
        ThreadManager::deleteTask(target);

        logDebug("%! (%i:%i) cancelled creation of process %i",
                 "syscall",
                 currentThread->process->main->id,
                 currentThread->id,
                 target->id);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: cancel process creation",
                "syscall",
                currentThread->process->main->id,
                currentThread->id);
    }

    return currentThread;
}

/**
 * Returns the id of the new process in creation
 */
SYSCALL_HANDLER(getCreatedProcessId) {
    Process*                    process = currentThread->process;
    SyscallGetCreatedProcessID* data
        = (SyscallGetCreatedProcessID*)SYSCALL_DATA(currentThread->cpuState);

    // Only on kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread* createdMainThread = (Thread*)(data->processObject);
        data->resultID            = createdMainThread->id;

        logDebug("%! (%i:%i) get id of created process: %i",
                 "syscall",
                 process->main->id,
                 currentThread->id,
                 createdMainThread->id);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: get created process id",
                "syscall",
                process->main->id,
                currentThread->id);
    }

    return currentThread;
}

/**
 * Writes the Thread Local Storage for the new process
 */
SYSCALL_HANDLER(writeTlsMasterForProcess) {
    Process*                         process = currentThread->process;
    SyscallWriteTlsMasterForProcess* data
        = (SyscallWriteTlsMasterForProcess*)SYSCALL_DATA(currentThread->cpuState);
    data->result = false;

    // Only on kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        // Get target main thread
        Thread*  targetThread  = (Thread*)(data->processObject);
        Process* targetProcess = targetThread->process;

        // Get a virtual address range for the TLS master copy
        uint32_t       requiredPages = PAGE_ALIGN_UP(data->copysize) / PAGE_SIZE;
        VirtualAddress tlsMasterVirt
            = targetProcess->virtualRanges.allocate(requiredPages,
                                                    PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER);

        // Temporarily copy master contents to kernel heap because we switch directories
        Local<uint8_t> temporaryStore(new uint8_t[data->copysize]);
        Memory::copy(temporaryStore(), data->content, data->copysize);
        uint32_t temporaryCopySize = data->copysize;

        // Temporarily switch to target process directory, map the pages & zero/copy content
        AddressSpace::switchToSpace(targetProcess->pageDirectory);
        for ( uint32_t i = 0; i < requiredPages; i++ ) {
            PhysicalAddress phys = PPallocator::allocate();
            AddressSpace::map(tlsMasterVirt + i * PAGE_SIZE,
                              phys,
                              DEFAULT_USER_TABLE_FLAGS,
                              DEFAULT_USER_PAGE_FLAGS);
            PPreferenceTracker::increment(phys);
        }
        Memory::copy((void*)tlsMasterVirt, temporaryStore(), temporaryCopySize);
        AddressSpace::switchToSpace(process->pageDirectory);

        // Write info to process
        targetProcess->tlsMasterInProcLocation = tlsMasterVirt;
        targetProcess->tlsMasterCopysize       = data->copysize;
        targetProcess->tlsMasterTotalsize      = data->totalsize;
        targetProcess->tlsMasterAlignment      = data->alignment;

        data->result = true;
        logDebug("%! (%i:%i) created tls master for process: %i, at: %h, size: %h, alignment: %h",
                 "syscall",
                 process->main->id,
                 currentThread->id,
                 targetThread->id,
                 targetProcess->tlsMasterInProcLocation,
                 targetProcess->tlsMasterCopysize,
                 targetProcess->tlsMasterAlignment);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: create tls space in process",
                "syscall",
                process->main->id,
                currentThread->id);
    }

    return currentThread;
}

/**
 * Performs a Spawning from the mounted ramdisk
 */
SYSCALL_HANDLER(ramdiskSpawn) {
    Process*             process = currentThread->process;
    SyscallRamdiskSpawn* data    = (SyscallRamdiskSpawn*)SYSCALL_DATA(currentThread->cpuState);

    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread*  spawnedThread;
        Process* spawnedProcess;

        Elf32SpawnStatus status = Elf32Loader::spawnFromRamdisk(data->path,
                                                                SECURITY_LEVEL_APPLICATION,
                                                                &spawnedThread,
                                                                false,
                                                                THREAD_PRIORITY_NORMAL);

        if ( status == Elf32SpawnStatus::SUCCESSFUL ) {
            data->spawnStatus = RAMDISK_SPAWN_STATUS_SUCCESSFUL;
            spawnedProcess    = spawnedThread->process;
            logDebug("%! (%i:%i) spawn '%s' from ramdisk -> %i <%h-%h> #%i",
                     "syscall",
                     process->main->id,
                     currentThread->id,
                     data->path,
                     spawnedProcess->imageStart,
                     spawnedProcess->imageEnd,
                     spawnedThread->id,
                     spawnedProcess->securityLevel);
        }

        else if ( status == Elf32SpawnStatus::FILE_NOT_FOUND ) {
            data->spawnStatus = RAMDISK_SPAWN_STATUS_FAILED_NOT_FOUND;
            logWarn("%! (%i:%i) error: file does not exist: spawn '%s' from ramdisk",
                    "syscall",
                    process->main->id,
                    currentThread->id,
                    data->path);
        }

        else {
            data->spawnStatus = RAMDISK_SPAWN_STATUS_FAILED_NOT_VALID;
            logWarn("%! task %i tried to spawn ramdisk binary '%s' but validation failed",
                    "syscall",
                    currentThread->id,
                    data->path);
            logWarn("%! (%i:%i) error: invalid elf: spawn '%s' from ramdisk",
                    "syscall",
                    process->main->id,
                    currentThread->id,
                    data->path);
        }
    }

    else {
        data->spawnStatus = RAMDISK_SPAWN_STATUS_FAILED_NOT_PERMITTED;
        logWarn("%! (%i:%i) error: insufficient permissions: spawn '%s' from ramdisk",
                "syscall",
                process->main->id,
                currentThread->id,
                data->path);
    }

    return currentThread;
}

/**
 * Returns the entry point for the current thread. If the executing task is not
 * a thread, zero is returned.
 */
SYSCALL_HANDLER(getThreadEntry) {
    SyscallGetThreadEntry* data = (SyscallGetThreadEntry*)SYSCALL_DATA(currentThread->cpuState);

    if ( currentThread->type == THREAD_TYPE_SUB ) {
        data->userEntry = currentThread->threadEntry;
        data->userData  = currentThread->userData;
        logDebug("%! process %i retrieved its thread entry %h",
                 "syscall",
                 currentThread->id,
                 currentThread->threadEntry);
    }

    else {
        data->userEntry = 0;
        logWarn("%! process %i tried to retrieve its thread entry but is a process",
                "syscall",
                currentThread->id);
    }

    return currentThread;
}

/**
 * Creates a thread. The given data must contain the initial entry library function
 * and the user entry. The user entry is then added to the thread data and is later
 * retrieved by the library function via getThreadEntry.
 */
SYSCALL_HANDLER(createThread) {
    SyscallCreateThread* data = (SyscallCreateThread*)SYSCALL_DATA(currentThread->cpuState);

    logDebug("%! (%i:%i) creates thread",
             "syscall",
             currentThread->process->main->id,
             currentThread->id);

    Thread* thread = ThreadManager::createSubThread(currentThread->process);
    if ( thread != 0 ) {
        logDebug("%! (%i:%i) created thread %i",
                 "syscall",
                 currentThread->process->main->id,
                 currentThread->id,
                 thread->id);
        thread->cpuState->eip = (uint32_t)data->initialEntry;
        thread->threadEntry   = data->userEntry;
        thread->userData      = data->userData;
        thread->setIdentifier(data->threadName);
        Tasking::addTask(thread);

        data->threadID = thread->id;
        data->status   = CREATE_THREAD_STATUS_SUCCESSFUL;
    }

    else {
        logWarn("%! (%i:%i) failed to create thread",
                "syscall",
                currentThread->process->main->id,
                currentThread->id);
        data->threadID = -1;
        data->status   = CREATE_THREAD_STATUS_FAILED;
    }

    // A process is forced to give away time when creating a thread
    return Tasking::schedule();
}

/**
 * Configure the path of the new process
 */
SYSCALL_HANDLER(configureProcess) {
    Process* process = currentThread->process;

    SyscallConfigureProcess* data = (SyscallConfigureProcess*)SYSCALL_DATA(currentThread->cpuState);
    data->result                  = false;

    // Only on kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread*  targetThread = (Thread*)data->processObject;
        Process* targetProc   = (targetThread)->process;
        targetProc->setPath(data->configuration.sourcePath);
        data->result = true;

        DEBUG_INTERFACE_TASK_SET_SOURCE_PATH(targetThread->id, data->configuration.sourcePath);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: configure process",
                "syscall",
                process->main->id,
                currentThread->id);
    }

    return currentThread;
}
