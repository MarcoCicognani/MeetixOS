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

#include "Api/Kernel.h"
#include "Api/utils/local.hpp"
#include "debug/DebugInterfaceKernel.hpp"

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
    data->m_process_creation_identifier = 0;

    Process* parentProcess = currentThread->process;

    // Only kernel level
    if ( parentProcess->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread* emptyMainThread
            = ThreadManager::createProcess(data->m_security_level, parentProcess);
        data->m_process_creation_identifier = emptyMainThread;

        logDebug("%! (%i:%i) created empty process %i",
                 "do_syscall",
                 parentProcess->main->id,
                 currentThread->id,
                 emptyMainThread->id);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: create empty process",
                "do_syscall",
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
        Thread*  other        = (Thread*)data->m_process_creation_identifier;
        Process* otherProcess = other->process;

        // allocate the command line buffer
        otherProcess->cliArguments = new char[CLIARGS_BUFFER_LENGTH];
        uint32_t argsLen           = StringUtils::length(data->m_arguments);
        Memory::copy(otherProcess->cliArguments, data->m_arguments, argsLen);
        otherProcess->cliArguments[argsLen] = 0;

        logDebug("%! task %i stored cli arguments for task %i",
                 "do_syscall",
                 currentThread->id,
                 other->id);
    }

    else {
        logWarn("%! task %i tried to store another tasks cli arguments but is not permitted",
                "do_syscall",
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
        auto args_len = StringUtils::length(process->cliArguments);

        Memory::copy(data->m_out_buffer, process->cliArguments, args_len);
        data->m_out_buffer[args_len] = 0;
        data->m_len = args_len;

        delete process->cliArguments;
        process->cliArguments = 0;
    }

    // Null-terminate in case of failure
    else {
        data->m_out_buffer[0] = 0;
        data->m_len=0;
    }

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
    data->m_shared_virtual_ptr = 0;

    // Only kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread*  targetThread  = (Thread*)data->m_process_creation_identifier;
        Process* targetProcess = targetThread->process;

        // Create page in target space
        // Below we do a temporary switch, where 'data' is not available (because we switch to the
        // target userspace, and 'data' is in the current userspace), so we copy the values:
        uint32_t virtualAddressInTargetSpace = data->m_target_space_virtual_address;
        uint32_t numberOfPages               = data->m_pages_count;

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
            PhysAddr physicalPages[numberOfPages];

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
            VirtAddr virtAddrHere
                = process->virtualRanges.allocate(numberOfPages, PROC_VIRTUAL_RANGE_FLAG_NONE);
            for ( uint32_t i = 0; i < numberOfPages; i++ )
                AddressSpace::map(virtAddrHere + i * PAGE_SIZE,
                                  physicalPages[i],
                                  DEFAULT_USER_TABLE_FLAGS,
                                  DEFAULT_USER_PAGE_FLAGS);

            data->m_shared_virtual_ptr = virtAddrHere;

            logDebug(
                "%! (%i:%i) created %i pages in space of process %i. virtual: here %h, there %h",
                "do_syscall",
                process->main->id,
                currentThread->id,
                data->numberOfPages,
                targetThread->id,
                virtAddrHere,
                data->m_target_space_virtual_address);
        }

        else {
            logWarn("%! (%i:%i) error: illegal arguments: tried to create pages in a space. %i is "
                    "not a valid number of pages (1-%i)",
                    "do_syscall",
                    process->main->id,
                    currentThread->id,
                    data->numberOfPages,
                    CREATE_PAGE_IN_SPACE_MAXIMUM_PAGES);
        }
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: create a page in a space",
                "do_syscall",
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
        Thread* targetMainThread        = (Thread*)data->m_process_creation_identifier;
        targetMainThread->cpuState->eip = data->m_instruction_ptr;
        ThreadManager::prepareThreadLocalStorage(targetMainThread);
        Tasking::addTask(targetMainThread);

        logDebug("%! (%i:%i) attached task %i, starting at eip %h",
                 "do_syscall",
                 currentThread->process->main->id,
                 currentThread->id,
                 targetMainThread->id,
                 targetMainThread->cpuState->eip);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: attach created process",
                "do_syscall",
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
        Thread* target = (Thread*)data->m_process_creation_identifier;
        ThreadManager::deleteTask(target);

        logDebug("%! (%i:%i) cancelled creation of process %i",
                 "do_syscall",
                 currentThread->process->main->id,
                 currentThread->id,
                 target->id);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: cancel process creation",
                "do_syscall",
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
        Thread* createdMainThread = (Thread*)(data->m_process_creation_identifier);
        data->m_proc_id           = createdMainThread->id;

        logDebug("%! (%i:%i) get id of created process: %i",
                 "do_syscall",
                 process->main->id,
                 currentThread->id,
                 createdMainThread->id);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: get created process id",
                "do_syscall",
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
    data->m_success = false;

    // Only on kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        // Get target main thread
        Thread*  targetThread  = (Thread*)(data->m_process_creation_identifier);
        Process* targetProcess = targetThread->process;

        // Get a virtual address range for the TLS master copy
        uint32_t requiredPages = PAGE_ALIGN_UP(data->m_copy_size) / PAGE_SIZE;
        VirtAddr tlsMasterVirt
            = targetProcess->virtualRanges.allocate(requiredPages,
                                                    PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER);

        // Temporarily copy master contents to kernel heap because we switch directories
        Local<uint8_t> temporaryStore(new uint8_t[data->m_copy_size]);
        Memory::copy(temporaryStore(), data->m_tls_content, data->m_copy_size);
        uint32_t temporaryCopySize = data->m_copy_size;

        // Temporarily switch to target process directory, map the pages & zero/copy content
        AddressSpace::switchToSpace(targetProcess->pageDirectory);
        for ( uint32_t i = 0; i < requiredPages; i++ ) {
            PhysAddr phys = PPallocator::allocate();
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
        targetProcess->tlsMasterCopysize       = data->m_copy_size;
        targetProcess->tlsMasterTotalsize      = data->m_total_size;
        targetProcess->tlsMasterAlignment      = data->m_alignment;

        data->m_success = true;
        logDebug("%! (%i:%i) created tls master for process: %i, at: %h, size: %h, alignment: %h",
                 "do_syscall",
                 process->main->id,
                 currentThread->id,
                 targetThread->id,
                 targetProcess->tlsMasterInProcLocation,
                 targetProcess->tlsMasterCopysize,
                 targetProcess->tlsMasterAlignment);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: create tls space in process",
                "do_syscall",
                process->main->id,
                currentThread->id);
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
        data->m_user_entry = currentThread->threadEntry;
        data->m_user_data  = currentThread->userData;
        logDebug("%! process %i retrieved its thread entry %h",
                 "do_syscall",
                 currentThread->id,
                 currentThread->threadEntry);
    }

    else {
        data->m_user_entry = 0;
        logWarn("%! process %i tried to retrieve its thread entry but is a process",
                "do_syscall",
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
             "do_syscall",
             currentThread->process->main->id,
             currentThread->id);

    Thread* thread = ThreadManager::createSubThread(currentThread->process);
    if ( thread != 0 ) {
        logDebug("%! (%i:%i) created thread %i",
                 "do_syscall",
                 currentThread->process->main->id,
                 currentThread->id,
                 thread->id);
        thread->cpuState->eip = (uint32_t)data->m_library_entry;
        thread->threadEntry   = data->m_user_entry;
        thread->userData      = data->m_user_data;
        thread->setIdentifier(data->m_thread_name);
        Tasking::addTask(thread);

        data->m_thread_id     = thread->id;
        data->m_thread_status = CREATE_THREAD_STATUS_SUCCESSFUL;
    }

    else {
        logWarn("%! (%i:%i) failed to create thread",
                "do_syscall",
                currentThread->process->main->id,
                currentThread->id);
        data->m_thread_id     = -1;
        data->m_thread_status = CREATE_THREAD_STATUS_FAILED;
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
    data->m_success               = false;

    // Only on kernel level
    if ( process->securityLevel == SECURITY_LEVEL_KERNEL ) {
        Thread*  targetThread = (Thread*)data->m_process_creation_identifier;
        Process* targetProc   = (targetThread)->process;
        targetProc->setPath(data->m_process_configuration.m_source_path);
        data->m_success = true;

        DEBUG_INTERFACE_TASK_SET_SOURCE_PATH(targetThread->id,
                                             data->m_process_configuration.m_source_path);
    }

    else {
        logWarn("%! (%i:%i) error: insufficient permissions: configure process",
                "do_syscall",
                process->main->id,
                currentThread->id);
    }

    return currentThread;
}
