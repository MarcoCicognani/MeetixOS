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
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <system/interrupts/handling/InterruptRequestHandler.hpp>
#include <system/interrupts/ioapic.hpp>
#include <system/interrupts/IoapicManager.hpp>
#include <system/interrupts/pic.hpp>
#include <system/IOPorts.hpp>
#include <system/system.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/wait/WaiterJoin.hpp>

/**
 * Map denoting which IRQs have happened and should be handled.
 */
bool        irqsWaiting[256] = {};
IrqHandler* handlers[256]    = {};

/**
 * Performs interrupt request handling
 *
 * @param currentThread:	the thread that be in execution
 * @return the next thread to be executed
 */
Thread* InterruptRequestHandler::handle(Thread* currentThread) {
    const uint32_t interrupt = currentThread->cpuState->intr;
    const uint32_t irq       = interrupt - 0x20;

    /**
     * System call
     * ===========
     * If the interrupt request is a system call, its handled by the system
     * call handler.
     */
    if ( currentThread->cpuState->intr == 0x80 )
        currentThread = SysCallHandler::handle(currentThread);

    /**
     * Spurious interrupt
     */
    else if ( irq == 0xFF ) {
        logWarn("%! spurious interrupt was caught", "requests");
    }

    /**
     * Timer request
     * =============
     * Scheduler is called. The milliseconds are updated separately, because the
     * method to schedule tasks may be called arbitrarily and is not guaranteed to be
     * called only on IRQ 0.
     */
    else if ( !irq ) {
        Tasking::currentScheduler()->updateMilliseconds();
        currentThread = Tasking::schedule();
    }

    /**
     * IRQ handling
     * ============
     * These requests are stored in the map of waiting IRQs. The IRQ is masked so it
     * doesnt happen again before it has been handled.
     */
    else if ( irq < 256 ) {
        IrqHandler* handler = handlers[irq];
        if ( handler ) {
            Thread* thread = Tasking::getTaskById(handler->threadID);
            if ( thread ) {
                // let the thread enter the irq handler
                thread->enterIrqHandler(handler->handler, irq, handler->callback);

                // it could be the current thread, so we have to switch
                currentThread = Tasking::schedule();
            }
        }

        // Mark the IRQ and mask it
        else
            irqsWaiting[irq] = true;
    }

    else {
        logWarn("%! unhandled irq %i", "requests", irq);
    }

    return currentThread;
}

/**
 * Checks if the given IRQ has happened and was not handled yet.
 *
 * @param irq:		the number of the irq to check
 * @return true if the irq has occured and should be handled
 */
bool InterruptRequestHandler::pollIrq(uint8_t irq) {
    if ( irqsWaiting[irq] ) {
        irqsWaiting[irq] = false;
        return true;
    }

    return false;
}

/**
 * create a new irq handler
 *
 * @param irq:				irq to handle
 * @param threadID:			the thread id that handle the irq
 * @param handlerAddr:		the handler address
 * @param callbackAddr:		the call back address
 */
void InterruptRequestHandler::setHandler(uint8_t   irq,
                                         Tid       threadID,
                                         uintptr_t handlerAddr,
                                         uintptr_t callbackAddr) {
    // if already exists destroy
    if ( handlers[irq] )
        delete handlers[irq];

    // create the new handler
    IrqHandler* handler = new IrqHandler;
    handler->threadID   = threadID;
    handler->handler    = handlerAddr;
    handler->callback   = callbackAddr;
    handlers[irq]       = handler;
}
