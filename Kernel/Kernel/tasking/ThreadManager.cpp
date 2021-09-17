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

#include "tasking/ThreadManager.hpp"

#include "eva/kernel.h"
#include "EvangelionNG.hpp"
#include "filesystem/filesystem.hpp"
#include "logger/logger.hpp"
#include "memory/AddressSpace.hpp"
#include "memory/collections/AddressRangePool.hpp"
#include "memory/collections/AddressStack.hpp"
#include "memory/constants.hpp"
#include "memory/gdt/GdtMacros.hpp"
#include "memory/LowerHeap.hpp"
#include "memory/memory.hpp"
#include "memory/physical/PPallocator.hpp"
#include "memory/physical/PPreferenceTracker.hpp"
#include "memory/TemporaryPagingUtil.hpp"
#include "system/interrupts/descriptors/ivt.hpp"
#include "tasking/communication/MessageController.hpp"
#include "tasking/process.hpp"
#include "tasking/tasking.hpp"
#include "utils/string.hpp"

/**
 * create a thread from process with provided type and return it's instance
 *
 * @param process:		the owner process of the thread
 * @param type:			the thread type (main/vm86/sub)
 * @return the instance of the thread
 */
Thread* ThreadManager::createThread(Process* process, ThreadType type) {
    // create the stacks
    VirtualAddress kernelStackPageVirt;
    if ( !createThreadKernelStack(process, &kernelStackPageVirt) )
        return 0;

    VirtualAddress userStackAreaStart;
    if ( !createThreadUserStack(process, &userStackAreaStart) )
        return 0;

    // calculate stack locations
    VirtualAddress esp0 = kernelStackPageVirt + PAGE_SIZE;
    VirtualAddress esp  = userStackAreaStart + PAGE_SIZE;

    // create initial state on the kernel stack
    ProcessorState* state = (ProcessorState*)(esp0 - sizeof(ProcessorState));
    Memory::setBytes(state, 0, sizeof(ProcessorState));
    state->esp    = esp;
    state->eip    = 0;
    state->eflags = 0x200;

    // apply security level configuration
    applySecurityLevel(state, process->securityLevel);

    // create the thread
    Thread* thread          = new Thread(type);
    thread->cpuState        = state;
    thread->kernelStackEsp0 = esp0;

    thread->kernelStackPageVirt = kernelStackPageVirt;
    thread->userStackAreaStart  = userStackAreaStart;
    thread->userStackPages      = 1;

    // link thread to process
    thread->process = process;

    // initialize thread local storage for subthreads
    if ( type == THREAD_TYPE_SUB )
        prepareThreadLocalStorage(thread);

    return thread;
}

/**
 * Allocates and initializes a new page directory used for process creation.
 *
 * @return the page directory forked
 */
PageDirectory ThreadManager::initializePageDirectoryForProcess() {
    PageDirectory currentPd = (PageDirectory)CONST_RECURSIVE_PAGE_DIRECTORY_ADDRESS;

    // allocate a page for the directory
    PhysicalAddress physPd = PPallocator::allocate();

    // temporarily map it
    PageDirectory tempPd = (PageDirectory)TemporaryPagingUtil::map(physPd);

    // clone mappings for all tables that are not in user range
    for ( uint32_t ti = 0; ti < 1024; ti++ ) {
        if ( !((currentPd[ti] & PAGE_ALIGN_MASK) & PAGE_TABLE_USERSPACE) )
            tempPd[ti] = currentPd[ti];
        else
            tempPd[ti] = 0;
    }

    // clone mappings for the lowest 4 MiB
    tempPd[0] = currentPd[0];

    // recursively map to self
    tempPd[1023] = physPd | DEFAULT_KERNEL_TABLE_FLAGS;

    // remove the temporary mapping
    TemporaryPagingUtil::unmap((VirtualAddress)tempPd);

    return (PageDirectory)physPd;
}

/**
 * fork the page directory for the created fork thread
 *
 * @param process:				the owner process
 * @param current:				the forked thread
 * @param outKernelStackVirt:	the forked kernel stack
 * @param outUserStackVirt:		the forked user stack
 * @return the physicalAddress of created directory
 */
PhysicalAddress ThreadManager::forkCurrentPageDirectory(Process*        process,
                                                        Thread*         sourceThread,
                                                        VirtualAddress* outKernelStackVirt,
                                                        VirtualAddress* outUserStackVirt) {
    PageDirectory currentPd = (PageDirectory)CONST_RECURSIVE_PAGE_DIRECTORY_ADDRESS;

    // create the directory
    PhysicalAddress physPd = PPallocator::allocate();

    // temporary map directory
    PageDirectory tempPd = (PageDirectory)TemporaryPagingUtil::map(physPd);

    // deep-copy
    for ( uint32_t ti = 1; ti < 1023; ti++ ) {
        if ( currentPd[ti] ) {
            // copy kernel tables
            if ( ti * 1024 * PAGE_SIZE >= CONST_KERNEL_AREA_START )
                tempPd[ti] = currentPd[ti]; // TODO make virtual ranges stay
            else if ( ti * 1024 * PAGE_SIZE >= CONST_USER_VIRTUAL_RANGES_START )
                tempPd[ti] = 0; // anything else is deep-copied
            else {
                // get address of the table
                PageTable table      = CONST_RECURSIVE_PAGE_TABLE(ti);
                uint32_t  tableFlags = currentPd[ti] & (PAGE_ALIGN_MASK);

                // create a new table
                PhysicalAddress clonedTablePhys = PPallocator::allocate();
                PageTable clonedTableTemp = (PageTable)TemporaryPagingUtil::map(clonedTablePhys);

                // copy page table entries
                for ( uint32_t pi = 0; pi < 1024; pi++ ) {
                    if ( table[pi] ) {
                        // clone page mappings as read-onle
                        clonedTableTemp[pi] = table[pi] & ~PAGE_TABLE_READWRITE;

                        // increment reference count on the physical page
                        PPreferenceTracker::increment(PAGE_ALIGN_DOWN(table[pi]));
                    }

                    else
                        clonedTableTemp[pi] = 0;
                }

                TemporaryPagingUtil::unmap((VirtualAddress)clonedTableTemp);

                // insert into new page directory
                tempPd[ti] = clonedTablePhys | tableFlags;
            }
        }

        else
            tempPd[ti] = 0;
    }

    // clone mappings for the lowest 4 MiB
    tempPd[0] = currentPd[0];

    // recursively map to self
    tempPd[1023] = physPd | DEFAULT_KERNEL_TABLE_FLAGS;

    // clone entire user stack area
    VirtualAddress userStackVirtRange
        = process->virtualRanges.allocate(THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES);
    VirtualAddress userStackStart
        = userStackVirtRange
        + (THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES - sourceThread->userStackPages) * PAGE_SIZE;

    for ( uint8_t i = 0; i < sourceThread->userStackPages; i++ ) {
        PhysicalAddress userStackPhys    = PPallocator::allocate();
        VirtualAddress  userStackPageOff = userStackStart + i * PAGE_SIZE;
        AddressSpace::mapToTemporaryMappedDirectory(tempPd,
                                                    userStackPageOff,
                                                    userStackPhys,
                                                    DEFAULT_USER_TABLE_FLAGS,
                                                    DEFAULT_USER_PAGE_FLAGS,
                                                    true);

        VirtualAddress userStackPageTemp = TemporaryPagingUtil::map(userStackPhys);
        Memory::copy((uint8_t*)userStackPageTemp,
                     (uint8_t*)(sourceThread->userStackAreaStart + i * PAGE_SIZE),
                     PAGE_SIZE);
        TemporaryPagingUtil::unmap(userStackPageTemp);
    }

    // unmap the temporary mapped directory
    TemporaryPagingUtil::unmap((VirtualAddress)tempPd);

    // copy kernel stack
    VirtualAddress  kernelStackVirt = EvaKernel::evaKernelRangePool->allocate(1);
    PhysicalAddress kernelStackPhys = PPallocator::allocate();

    AddressSpace::map(kernelStackVirt,
                      kernelStackPhys,
                      DEFAULT_KERNEL_TABLE_FLAGS,
                      DEFAULT_KERNEL_PAGE_FLAGS);
    Memory::copy((uint8_t*)kernelStackVirt, (uint8_t*)sourceThread->kernelStackPageVirt, PAGE_SIZE);

    // modify the forked directory
    for ( uint32_t ti = 1; ti < 1023; ti++ ) {
        if ( currentPd[ti] ) {
            // process area
            if ( ti * 1024 * PAGE_SIZE < CONST_USER_VIRTUAL_RANGES_START ) {
                // make all pages in all tables read-only
                PageTable table = CONST_RECURSIVE_PAGE_TABLE(ti);
                for ( uint32_t pi = 0; pi < 1024; pi++ )
                    table[pi] = table[pi] & ~PAGE_TABLE_READWRITE;

                // virtual ranges
            }

            else if ( ti * 1024 * PAGE_SIZE
                      < CONST_KERNEL_AREA_START ) { /* TODO what to do with virtual ranges? */
            }
        }
    }

    *outKernelStackVirt = kernelStackVirt;
    *outUserStackVirt   = userStackStart;

    return physPd;
}

/**
 * Allocates the kernel thread stack for a new thread in the given process.
 *
 * @param process:				the owner process of stack
 * @param outUserStackVirt:		the created user stack
 * @return if the operation end fine
 */
bool ThreadManager::createThreadKernelStack(Process* process, VirtualAddress* outKernelStackVirt) {
    // perform stack mapping
    VirtualAddress kernelStackVirt = EvaKernel::evaKernelRangePool->allocate(1);
    if ( !kernelStackVirt ) {
        if ( process->main ) {
            logWarn("%! thread creation for process %i failed: kernel virtual ranges are full",
                    "Thread Manager",
                    process->main->id);
        }

        else {
            logWarn("%! main thread creation failed: kernel virtual ranges are full",
                    "Thread Manager");
        }
        return false;
    }

    // allocate physical locations
    PhysicalAddress kernelStackPhys = PPallocator::allocate();

    // map kernel stack (global space)
    AddressSpace::map(kernelStackVirt,
                      kernelStackPhys,
                      DEFAULT_KERNEL_TABLE_FLAGS,
                      DEFAULT_KERNEL_PAGE_FLAGS);

    // set out parameters
    *outKernelStackVirt = kernelStackVirt;
    return true;
}

/**
 * Allocates the user thread stack for a new thread in the given process.
 *
 * @param process:				the owner process of stack
 * @param outUserStackVirt:		the created user stack
 * @return if the operation end fine
 */
bool ThreadManager::createThreadUserStack(Process* process, VirtualAddress* outUserStackVirt) {
    // prepare user stack virtual range and address
    VirtualAddress userStackVirtRange
        = process->virtualRanges.allocate(THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES);
    if ( !userStackVirtRange ) {
        if ( process->main ) {
            logWarn("%! thread creation for process %i failed: no virtual ranges available for "
                    "stack allocation",
                    "Thread Manager",
                    process->main->id);
        }

        else {
            logWarn(
                "%! main thread creation failed: no virtual ranges available for stack allocation",
                "Thread Manager");
        }

        return false;
    }

    // user stack is at the end of the range
    VirtualAddress userStackVirt
        = userStackVirtRange + (THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES - 1) * PAGE_SIZE;

    // allocate physical locations
    PhysicalAddress userStackPhys = PPallocator::allocate();

    // map directory temporary and map user stack
    PageDirectory tempPd
        = (PageDirectory)TemporaryPagingUtil::map((PhysicalAddress)process->pageDirectory);
    AddressSpace::mapToTemporaryMappedDirectory(tempPd,
                                                userStackVirt,
                                                userStackPhys,
                                                DEFAULT_USER_TABLE_FLAGS,
                                                DEFAULT_USER_PAGE_FLAGS);
    TemporaryPagingUtil::unmap((VirtualAddress)tempPd);

    // set out parameters
    *outUserStackVirt = userStackVirt;
    return true;
}

/**
 * Frees and unmap the memories between the provided addresses
 *
 * @param start:		the start address
 * @param end:			the end address
 * @param ranges:		the pointer to the address range pool of the process, if 0 is provided
 * isn't used
 */
void ThreadManager::freeAndUnmap(VirtualAddress    start,
                                 VirtualAddress    end,
                                 AddressRangePool* ranges) {
    // parse all the addresses
    for ( VirtualAddress address = start; address < end; address += PAGE_SIZE ) {
        // free the physical page
        PhysicalAddress paddress = AddressSpace::virtualToPhysical(address);
        PPreferenceTracker::decrement(paddress);
        PPallocator::free(paddress);

        // unmap the page
        AddressSpace::unmap(address);
        if ( ranges )
            ranges->free(address);
    }
}

/**
 * show all properties of provided task (used in debug cases)
 *
 * @param task:		the task that wqe have to dump
 */
void ThreadManager::dumpTask(Thread* task) {
#if LOGGING_DEBUG
    logDebug("%! created %s %i",
             "Thread Manager",
             (task->type == THREAD_TYPE_SUB
                  ? "thread"
                  : (task->type == THREAD_TYPE_VM86 ? "vm86 process" : "process")),
             task->id);

    Process* process = task->process;
    logDebugn("%#  process: ");
    if ( task->process ) {
        logDebug("%i", process->main->id);
    }

    else {
        logDebug("none");
    }

    logDebug("%#  pagedir:  %h", process->pageDirectory);
    logDebug("%#  security: %h", process->securityLevel);
    logDebug("%#  kernel sp:     %h", task->kernelStackEsp0);
    if ( task->type == THREAD_TYPE_VM86 ) {
        logDebug("%#  cs:ip:  %h:%h", task->cpuState->cs, task->cpuState->eip);
        logDebug("%#  ax: %h bx: %h cx: %h dx: %h",
                 task->cpuState->eax,
                 task->cpuState->ebx,
                 task->cpuState->ecx,
                 task->cpuState->edx);
        logDebug("%#  user sp:  %h:%h", task->cpuState->esp, task->cpuState->ss);
    }

    else {
        logDebug("%#  user sp:       %h", task->cpuState->esp);
        logDebug("%#  entry point:   %h", task->cpuState->eip);
    }
#endif
}

/**
 * create a new process with the provided SecurityLevel and linked with the parent
 *
 * @param securityLevel:	the SecurityLevel of the process
 * @param parent:			the parent process
 * @return the created main thread of the process
 */
Thread* ThreadManager::createProcess(SecurityLevel securityLevel, Process* parent) {
    // create the process
    Process* process       = new Process(securityLevel);
    process->pageDirectory = initializePageDirectoryForProcess();
    process->virtualRanges.initialize(CONST_USER_VIRTUAL_RANGES_START, CONST_KERNEL_AREA_START);

    // create main thread
    Thread* mainThread = createThread(process, THREAD_TYPE_MAIN);
    process->parent    = parent;
    process->main      = mainThread;

#if LOGGING_DEBUG
    dumpTask(mainThread);
#endif
    return mainThread;
}

/**
 * create a virtual 8086 process
 *
 * @param interrupt:	the interrupt code
 * @param in:			the provided Vm86Registers instace
 * @param out:			the returned informations
 * @return the created main thread of the process
 */
Thread* ThreadManager::createProcessVm86(uint8_t interrupt, Vm86Registers& in, Vm86Registers* out) {
    Process* process       = new Process(SECURITY_LEVEL_KERNEL);
    process->pageDirectory = initializePageDirectoryForProcess();

    // create kernel stack
    VirtualAddress kernelStackVirt;
    if ( !createThreadKernelStack(process, &kernelStackVirt) )
        return 0;

    // allocate user stack in lower memory
    VirtualAddress userStackVirt = (uint32_t)LowerHeap::allocate(0x2000);

    // initialize the state
    VirtualAddress esp0 = kernelStackVirt + PAGE_SIZE;

    ProcessorStateVm86* state = (ProcessorStateVm86*)(esp0 - sizeof(ProcessorStateVm86));
    Memory::setBytes(state, 0, sizeof(ProcessorStateVm86));
    state->defaultFrame.eax = in.ax;
    state->defaultFrame.ebx = in.bx;
    state->defaultFrame.ecx = in.cx;
    state->defaultFrame.edx = in.dx;
    state->defaultFrame.ebp = 0;
    state->defaultFrame.esi = in.si;
    state->defaultFrame.edi = in.di;

    state->defaultFrame.eip    = FP_OFF(ivt->entry[interrupt]);
    state->defaultFrame.cs     = FP_SEG(ivt->entry[interrupt]);
    state->defaultFrame.eflags = 0x20202;
    state->defaultFrame.esp    = 0x1000;
    state->defaultFrame.ss     = (((userStackVirt & ~(0xFFF)) + 0x1000) >> 4);

    state->gs = 0x00;
    state->fs = 0x00;
    state->es = in.es;
    state->ds = in.ds;

    // create main thread
    Thread* thread          = new Thread(THREAD_TYPE_VM86);
    thread->cpuState        = (ProcessorState*)state;
    thread->kernelStackEsp0 = esp0;

    thread->kernelStackPageVirt = kernelStackVirt;
    thread->userStackAreaStart  = userStackVirt;
    thread->userStackPages      = 1;

    thread->getVm86Information()->out = out;

    // assign thread to process
    process->main   = thread;
    thread->process = process;

#if LOGGING_DEBUG
    dumpTask(thread);
#endif

    return thread;
}

/**
 * create a new subthread of the provided process
 *
 * @param process:		the process that is the owner of the new thread
 * @return the new created thread
 */
Thread* ThreadManager::createSubThread(Process* process) {
    Thread* thread = createThread(process, THREAD_TYPE_SUB);

#if LOGGING_DEBUG
    dumpTask(thread);
#endif

    return thread;
}

/**
 * apply the SecurityLevel to cpu state
 *
 * @param state:			cpu state instance
 * @param securityLevel:	the security level to apply
 */
void ThreadManager::applySecurityLevel(ProcessorState* state, SecurityLevel securityLevel) {
    // apply special permission only for kernel processes
    if ( securityLevel == SECURITY_LEVEL_KERNEL ) {
        state->cs = GDT_DESCRIPTOR_KERNEL_CODE | SEGMENT_SELECTOR_RING0;
        state->ss = GDT_DESCRIPTOR_KERNEL_DATA | SEGMENT_SELECTOR_RING0;
        state->ds = GDT_DESCRIPTOR_KERNEL_DATA | SEGMENT_SELECTOR_RING0;
        state->es = GDT_DESCRIPTOR_KERNEL_DATA | SEGMENT_SELECTOR_RING0;
        state->fs = GDT_DESCRIPTOR_KERNEL_DATA | SEGMENT_SELECTOR_RING0;
        state->gs = GDT_DESCRIPTOR_KERNEL_DATA | SEGMENT_SELECTOR_RING0;
    }

    // otherwise apply only a user permission
    else {
        state->cs = GDT_DESCRIPTOR_USER_CODE | SEGMENT_SELECTOR_RING3;
        state->ss = GDT_DESCRIPTOR_USER_DATA | SEGMENT_SELECTOR_RING3;
        state->ds = GDT_DESCRIPTOR_USER_DATA | SEGMENT_SELECTOR_RING3;
        state->es = GDT_DESCRIPTOR_USER_DATA | SEGMENT_SELECTOR_RING3;
        state->fs = GDT_DESCRIPTOR_USER_DATA | SEGMENT_SELECTOR_RING3;
        state->gs = GDT_DESCRIPTOR_USER_DATA | SEGMENT_SELECTOR_RING3;
    }

    // enable special I/O for process from driver
    if ( securityLevel <= SECURITY_LEVEL_SERVER )
        state->eflags |= 0x3000; // IOPL 3
}

/**
 * fork the provided process from it's main thread
 *
 * @param task:		the thread to be forked
 * @return the forked thread
 */
Thread* ThreadManager::fork(Thread* sourceThread) {
    Process* parent  = sourceThread->process;
    Process* process = new Process(parent->securityLevel);
    process->virtualRanges.initialize(
        CONST_USER_VIRTUAL_RANGES_START,
        CONST_KERNEL_AREA_START); // TODO clone virtual ranges during forking

    VirtualAddress kernelStackVirt;
    VirtualAddress userStackVirt;
    process->pageDirectory = (PageDirectory)
        forkCurrentPageDirectory(process, sourceThread, &kernelStackVirt, &userStackVirt);

    // copy heap information
    process->heapBreak  = parent->heapBreak;
    process->heapPages  = parent->heapPages;
    process->heapStart  = parent->heapStart;
    process->imageEnd   = parent->imageEnd;
    process->imageStart = parent->imageStart;

    // create main thread
    VirtualAddress esp0 = kernelStackVirt + PAGE_SIZE;

    Thread* thread          = new Thread(THREAD_TYPE_MAIN);
    thread->cpuState        = (ProcessorState*)(esp0 - sizeof(ProcessorState));
    thread->kernelStackEsp0 = esp0;

    thread->kernelStackPageVirt = kernelStackVirt;
    thread->userStackAreaStart  = userStackVirt;
    thread->userStackPages      = sourceThread->userStackPages;

    // link thread to process
    process->parent = parent;
    process->main   = thread;
    thread->process = process;

#if LOGGING_DEBUG
    dumpTask(thread);
#endif
    return thread;
}

/**
 * prepare the local storage for the provided thread
 *
 * @param thread:		the thread that shall stored
 */
void ThreadManager::prepareThreadLocalStorage(Thread* thread) {
    // if tls master copy available, copy it to thread
    Process* process = thread->process;
    if ( process->tlsMasterInProcLocation ) {
        // calculate size that TLS needs including alignment
        uint32_t tlsMasterAlignedTotalAize
            = ALIGN_UP(process->tlsMasterTotalsize, process->tlsMasterAlignment);

        // allocate virtual range with aligned size of TLS + size of {UserThread}
        uint32_t       requiredSize  = tlsMasterAlignedTotalAize + sizeof(UserThread);
        uint32_t       requiredPages = PAGE_ALIGN_UP(requiredSize) / PAGE_SIZE;
        VirtualAddress tlsCopyVirt
            = process->virtualRanges.allocate(requiredPages,
                                              PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER);

        // store executing space
        PageDirectory current = AddressSpace::getCurrentSpace();

        // temporarily switch to target process directory, copy TLS contents
        AddressSpace::switchToSpace(process->pageDirectory);
        for ( uint32_t i = 0; i < requiredPages; i++ ) {
            PhysicalAddress phys = PPallocator::allocate();
            AddressSpace::map(tlsCopyVirt + i * PAGE_SIZE,
                              phys,
                              DEFAULT_USER_TABLE_FLAGS,
                              DEFAULT_USER_PAGE_FLAGS);
            PPreferenceTracker::increment(phys);
        }

        // zero & copy TLS content
        Memory::setBytes((void*)tlsCopyVirt, 0, process->tlsMasterTotalsize);
        Memory::copy((void*)tlsCopyVirt,
                     (void*)process->tlsMasterInProcLocation,
                     process->tlsMasterCopysize);

        // fill user thread
        VirtualAddress userThreadLoc = tlsCopyVirt + tlsMasterAlignedTotalAize;
        UserThread*    userThread    = (UserThread*)userThreadLoc;
        userThread->self             = userThread;

        // switch back
        AddressSpace::switchToSpace(current);

        // set threads TLS location
        thread->userThreadAddr = userThreadLoc;
        thread->tlsCopyVirt    = tlsCopyVirt;

        logDebug("%! created tls copy in process %i, thread %i at %h",
                 "Thread Manager",
                 process->main->id,
                 thread->id,
                 thread->tlsCopyVirt);
    }

    // setup initial identifier
    setupInitialIdentifier(thread, thread->type);
}

/**
 * setup the initial identifier for the task from type
 *
 * @param task:		the task that be setted
 * @param type:		the thread type (main/vm86/sub)
 */
void ThreadManager::setupInitialIdentifier(Thread* task, ThreadType type) {
    // setup name for main type of thread
    if ( type == THREAD_TYPE_MAIN || type == THREAD_TYPE_VM86 ) {
        // get name of task from basename of path
        char* name  = String::basename(task->process->sourcePath);
        int   index = String::indexOf(name, '.');
        if ( index == -1 )
            task->setIdentifier(name);
        else {
            // get length of extension of file
            char* ext     = String::getOccurenceOf(name, '.');
            int   extlen  = String::length(ext);
            int   namelen = String::length(name);

            // erase extension
            if ( extlen < namelen ) {
                char purename[namelen - extlen];
                Memory::copy(purename, name, namelen - extlen);
                purename[namelen - extlen] = 0;
                task->setIdentifier(purename);
            }
        }
    }

    // on subthread get the name from main thread of process and add "::"
    else if ( type == THREAD_TYPE_SUB ) {
        const char* mainName = task->process->main->getIdentifier();
        int         len      = String::length(mainName);

        char threadname[len + 2];
        String::concat(mainName, "::", threadname);

        task->setIdentifier(threadname);
    }
}

/**
 * deletes a task and cleans up all its memory. This task must not be in the
 * scheduling list anymore.
 *
 *	@param task:		the task to destroy
 */
void ThreadManager::deleteTask(Thread* thread) {
    // get the page for security
    PageDirectory currentSpace = AddressSpace::getCurrentSpace();

    // clear message queues
    MessageController::clear(thread->id);

    // remove kernel stack
    PPallocator::free(AddressSpace::virtualToPhysical(thread->kernelStackPageVirt));

    // remove the user stack with the lowerHeap manager on VM86 tasks
    if ( thread->type == THREAD_TYPE_VM86 )
        LowerHeap::free((void*)thread->userStackAreaStart);

    // remove user stack as a normal stack
    else {
        AddressSpace::switchToSpace(thread->process->pageDirectory);
        freeAndUnmap(thread->userStackAreaStart,
                     thread->userStackAreaStart + thread->userStackPages * PAGE_SIZE,
                     0);
        AddressSpace::switchToSpace(currentSpace);
    }

    // only if is avaible a TLS copy
    if ( thread->tlsCopyVirt ) {
        AddressSpace::switchToSpace(thread->process->pageDirectory);
        freeAndUnmap(thread->tlsCopyVirt,
                     thread->process->tlsMasterTotalsize,
                     &thread->process->virtualRanges);
        AddressSpace::switchToSpace(currentSpace);
    }

    // we have to destroy the entire process
    if ( thread->type == THREAD_TYPE_MAIN ) {
        // get the process object
        Process* process = thread->process;

        // ask to filesystem to close file reference of process
        FileSystem::processClosed(process->main->id);

        // deallocate the process image, heap and TLS
        AddressSpace::switchToSpace(process->pageDirectory);
        freeAndUnmap(process->imageStart, process->imageEnd, 0);
        freeAndUnmap(process->heapStart, process->heapBreak, 0);
        freeAndUnmap(process->tlsMasterInProcLocation,
                     process->tlsMasterTotalsize,
                     &process->virtualRanges);
        AddressSpace::switchToSpace(currentSpace);

        // bye bye process
        delete process;
    }

    // destroy the thread descriptor
    delete thread;
}

/**
 * return the used heap amount of the process
 *
 * @param task:		the task that we would get the memory amount
 * @return the memory amount in kb
 */
uint32_t ThreadManager::getProcessHeapSize(Thread* task) {
    return (task->process->heapPages * PAGE_SIZE) / 1024;
}

/**
 * return the size of the process in memory from it's memory image
 *
 * @param task:		one the process tasks
 * @return the size of the process on memory kb
 */
uint32_t ThreadManager::getProcessImageSize(Thread* task) {
    return (((task->process->imageEnd - task->process->imageStart) * PAGE_SIZE) / 1024);
}

/**
 * returns the thread stack size in kb
 *
 * @param task:		one the process tasks
 * @return the size of the process on memory kb
 */
uint32_t ThreadManager::getThreadStackSize(Thread* task) {
    return ((task->userStackPages + 1) * PAGE_SIZE / 1024);
}
