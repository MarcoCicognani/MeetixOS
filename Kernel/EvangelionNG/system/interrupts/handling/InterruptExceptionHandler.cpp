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

#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/constants.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/physical/PPreferenceTracker.hpp>
#include <memory/TemporaryPagingUtil.hpp>
#include <system/interrupts/handling/InterruptExceptionHandler.hpp>
#include <system/system.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <vm86/Virtual8086Monitor.hpp>

/**
 * Names of the exceptions
 */
static const char* EXCEPTION_NAMES[] = {
    "divide error",                     // 0x00
    "debug exception",                  // 0x01
    "non-maskable interrupt exception", // 0x02
    "breakpoint exception",             // 0x03
    "detected overflow",                // 0x04
    "out of bounds",                    // 0x05
    "invalid operation code",           // 0x06
    "no co-processor",                  // 0x07
    "double fault",                     // 0x08
    "co-processor segment overrun",     // 0x09
    "Bad TSS exception",                // 0x0A
    "segment not present",              // 0x0B
    "stack fault",                      // 0x0C
    "general protection fault",         // 0x0D
    "page fault",                       // 0x0E
    "unknown interrupt",                // 0x0F
    "co-processor fault",               // 0x10
    "alignment check exception",        // 0x11
    "machine check exception",          // 0x12
    "reserved exception",               // 0x13
    "reserved exception",               // 0x14
    "reserved exception",               // 0x15
    "reserved exception",               // 0x16
    "reserved exception",               // 0x17
    "reserved exception",               // 0x18
    "reserved exception",               // 0x19
    "reserved exception",               // 0x1A
    "reserved exception",               // 0x1B
    "reserved exception",               // 0x1C
    "reserved exception",               // 0x1D
    "reserved exception",               // 0x1E
    "reserved exception",               // 0x1F
};

/**
 * handle by code the types of execeptions
 *
 * @param currentThread:	the thread that have caused the exception
 * @return a new thread if the exeception is resolved, healt the system otherwise
 */
Thread* InterruptExceptionHandler::handle(Thread* currentThread) {
    // lock the logger
    Logger::manualLock();

    // check the exception type and try to resolve it
    bool resolved = false;
    switch ( currentThread->cpuState->intr ) {
        // Divide error
        case 0x00:
            {
                currentThread = handleDivideError(currentThread);
                resolved      = true;
                break;
            }

        // Page fault
        case 0x0E:
            {
                currentThread = handlePageFault(currentThread);
                resolved      = true;
                break;
            }

        // General protection fault
        case 0x0D:
            {
                currentThread = handleGeneralProtectionFault(currentThread);
                resolved      = true;
                break;
            }

        // Invalid operation code
        case 0x06:
            {
                currentThread = handleInvalidOperationCode(currentThread);
                resolved      = true;
                break;
            }
    }

    // if exception is resolved continue the execution
    if ( resolved ) {
        Logger::manualUnlock();
        return currentThread;
    }

    // very bad moment bye bye
    logInfo("%*%! no resolution, hanging system", 0x0C, "exception");
    dump(currentThread);
    while ( true )
        asm("hlt");
    return nullptr;
}

/**
 * Handles a general protection fault. If the current task is a VM86 task, the fault is redirected
 * to the Virtual8086 monitor.
 *
 * @param currentThread:		the thread that have caused the exception
 * @return a new thread
 */
Thread* InterruptExceptionHandler::handleGeneralProtectionFault(Thread* currentThread) {
    // redirect the handle to the vm86 monitor if task is a vm86
    if ( currentThread->type == THREAD_TYPE_VM86 ) {
        VMResult result = Virtual8086Monitor::handleGpf(currentThread);
        switch ( result ) {
            // success
            case VMResult::SUCCESSFUL:
                return currentThread;

            // task termination
            case VMResult::FINISHED:
                currentThread->alive = false;
                return Tasking::schedule();

            // bad moment
            case VMResult::UNHANDLED_OPCODE:
                logInfo("%! %i unable to handle gpf for vm86 task",
                        "exception",
                        System::currentProcessorId());
                currentThread->alive = false;
                return Tasking::schedule();
        }
    }

    // kill process, return with a switch
    currentThread->process->main->alive = false;
    dump(currentThread);
    logInfo("%! #%i process %i killed due to general protection fault",
            "exception",
            System::currentProcessorId(),
            currentThread->process->main->id);
    return Tasking::schedule();
}

/**
 * handle the page fault execeptions
 *
 * @param currentThread:		the thread that have caused the exception
 * @return a new thread
 */
Thread* InterruptExceptionHandler::handlePageFault(Thread* currentThread) {
    VirtualAddress  accessedVirtual  = PAGE_ALIGN_DOWN(getCR2());
    PhysicalAddress accessedPhysical = AddressSpace::virtualToPhysical(accessedVirtual);

    // handle-able stack overflow?
    if ( currentThread->type == THREAD_TYPE_SUB || currentThread->type == THREAD_TYPE_MAIN ) {
        // calculate the virtual stack area
        VirtualAddress stackAreaStart
            = currentThread->userStackAreaStart
            - (THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES - currentThread->userStackPages)
                  * PAGE_SIZE;
        VirtualAddress stackAreaEnd
            = currentThread->userStackAreaStart + currentThread->userStackPages * PAGE_SIZE;

        if ( accessedVirtual >= stackAreaStart && accessedVirtual < stackAreaEnd ) {
            // start at the accessed page
            VirtualAddress unmappedNext = PAGE_ALIGN_DOWN(accessedVirtual);
            while ( unmappedNext < currentThread->userStackAreaStart ) {
                // map physical pages until mapped stack-start is reached
                PhysicalAddress addPagePhys = PPallocator::allocate();
                AddressSpace::map(unmappedNext,
                                  addPagePhys,
                                  DEFAULT_USER_TABLE_FLAGS,
                                  DEFAULT_USER_PAGE_FLAGS);

                unmappedNext = unmappedNext + PAGE_SIZE;
                currentThread->userStackPages++;
            }

            // set area to new location
            currentThread->userStackAreaStart = PAGE_ALIGN_DOWN(accessedVirtual);

            // continue
            return currentThread;
        }
    }

    // Copy-on-write?
    // Check if within binary image range
    if ( accessedVirtual >= currentThread->process->imageStart
         && accessedVirtual <= currentThread->process->imageEnd ) {
        uint32_t  ti    = TABLE_IN_DIRECTORY_INDEX(accessedVirtual);
        uint32_t  pi    = PAGE_IN_TABLE_INDEX(accessedVirtual);
        PageTable table = CONST_RECURSIVE_PAGE_TABLE(ti);
        if ( table[pi] != 0 ) {
            // get new physical page
            PhysicalAddress newPhysPhysical = PPallocator::allocate();
            // map it temporary
            VirtualAddress newPhysTemp = TemporaryPagingUtil::map(newPhysPhysical);
            // copy contents
            Memory::copy((uint8_t*)newPhysTemp, (uint8_t*)accessedVirtual, PAGE_SIZE);
            // write new mapping
            AddressSpace::map(accessedVirtual,
                              newPhysPhysical,
                              DEFAULT_USER_TABLE_FLAGS,
                              DEFAULT_USER_PAGE_FLAGS,
                              true);
            // unmap temporary
            TemporaryPagingUtil::unmap(newPhysTemp);

            // new physical page has one more reference, old one has one less
            PPreferenceTracker::increment(newPhysPhysical);
            if ( !PPreferenceTracker::decrement(accessedPhysical) )
                PPallocator::free(accessedPhysical);

            logDebug("%! (%i:%i) entry %i/%i copied",
                     "cow",
                     currentThread->process->main->id,
                     currentThread->id,
                     ti,
                     pi);
            return currentThread;
        }
    }

    // raise SIGSEGV in thread
    currentThread->raiseSignal(SIGSEGV);
    logInfo("%! (core %i) raised SIGSEGV in thread %i",
            "pagefault",
            System::currentProcessorId(),
            currentThread->id);
    dump(currentThread);

    auto ebp = reinterpret_cast<VirtualAddress*>(currentThread->cpuState->ebp);
    logInfo("%# stack trace:");
    for ( int frame = 0; frame < 5; ++frame ) {
        auto eip = ebp[1];
        if ( eip == 0 ) {
            break;
        }
        ebp = reinterpret_cast<VirtualAddress*>(ebp[0]);
        logInfo("%#  %h", eip);
    }

    return Tasking::schedule();
}

/**
 * handle dividing execeptions
 *
 * @param currentThread:		the thread that have caused the exception
 * @return a new thread
 */
Thread* InterruptExceptionHandler::handleDivideError(Thread* currentThread) {
    dump(currentThread);

    // Let process run, but skip the faulty instruction
    ++currentThread->cpuState->eip;
    logInfo("%! #%i thread %i had a divide error",
            "exception",
            System::currentProcessorId(),
            currentThread->id);

    return Tasking::schedule();
}

/**
 * handle the invalid operations code execeptions
 *
 * @param currentThread:		the thread that have caused the exception
 * @return a new thread
 */
Thread* InterruptExceptionHandler::handleInvalidOperationCode(Thread* currentThread) {
    dump(currentThread);

    // kill thread and process, return with a switch
    currentThread->process->main->alive = false;
    currentThread->alive                = false;
    logInfo("%! #%i process %i killed due to invalid operation code %h in thread %i",
            "exception",
            System::currentProcessorId(),
            currentThread->process->main->id,
            *((uint8_t*)currentThread->cpuState->eip),
            currentThread->id);

    return Tasking::schedule();
}

/**
 * @return the cr2 regex value
 */
uint32_t InterruptExceptionHandler::getCR2() {
    uint32_t addr;
    asm volatile("mov %%cr2, %0" : "=r"(addr));
    return addr;
}

/**
 * print dump informations of the provided thread
 *
 * @param currentThread:		the thread to be dumped
 */
void InterruptExceptionHandler::dump(Thread* currentThread) {
    ProcessorState* cpuState = currentThread->cpuState;
    Process*        process  = currentThread->process;
    logInfo("%! %s in task %i (process %i)",
            "exception",
            EXCEPTION_NAMES[cpuState->intr],
            currentThread->id,
            process->main->id);

    if ( currentThread->getIdentifier() != 0 ) {
        logInfo("%# task identified as '%s'", currentThread->getIdentifier());
    }

    // Page fault
    if ( cpuState->intr == 0x0E ) {
        logInfo("%#    accessed address: %h", getCR2());
    }
    logInfo("%#    eip: %h   eflags: %h", cpuState->eip, cpuState->eflags);
    logInfo("%#    eax: %h      ebx: %h", cpuState->eax, cpuState->ebx);
    logInfo("%#    ecx: %h      edx: %h", cpuState->ecx, cpuState->edx);
    logInfo("%#    esp: %h   state@: %h", cpuState->esp, cpuState);
    logInfo("%#   intr: %h    error: %h", cpuState->intr, cpuState->error);
}

/**
 * print the cpu state
 *
 * @param cpuState:		the cpu state to be printed
 */
void InterruptExceptionHandler::printStackTrace(ProcessorState* state) {
    // This function is not very safe, use with caution.
    // Disallowed in anything but debug mode.
#if LOG_LEVEL == LOG_LEVEL_DEBUG
    logInfo("%! Stack trace:", "exception");

    uint32_t* ebp = (uint32_t*)state->ebp;
    for ( uint32_t frame = 0; frame < 5; ++frame ) {
        uint32_t eip = ebp[1];
        if ( eip == 0 )
            break;

        ebp = reinterpret_cast<uint32_t*>(ebp[0]);
        // uint32_t* arguments = &ebp[2];
        logInfo("%#  %h", eip);
    }
#endif
}
