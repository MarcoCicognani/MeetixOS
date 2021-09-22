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

#ifndef EVA_INTERRUPTS_REQUEST_HANDLERS
#define EVA_INTERRUPTS_REQUEST_HANDLERS

#include "Api/Kernel.h"
#include "Api/StdInt.h"

#include <system/ProcessorState.hpp>
#include <tasking/thread.hpp>

/**
 * Type of an interrupt handler
 */
struct IrqHandler {
    Tid       threadID;
    uintptr_t handler;
    uintptr_t callback;
};

/**
 * Interrupt request handling class
 */
class InterruptRequestHandler {
public:
    /**
     * Performs interrupt request handling
     *
     * @param currentThread:	the thread that be in execution
     * @return the next thread to be executed
     */
    static Thread* handle(Thread* currentThread);

    /**
     * Checks if the given IRQ has happened and was not handled yet.
     *
     * @param irq:		the number of the irq to check
     * @return true if the irq has occured and should be handled
     */
    static bool pollIrq(uint8_t irq);

    /**
     * create a new irq handler
     *
     * @param irq:				irq to handle
     * @param threadID:			the thread id that handle the irq
     * @param handlerAddr:		the handler address
     * @param callbackAddr:		the call back address
     */
    static void
    setHandler(uint8_t irq, Tid threadID, uintptr_t handlerAddr, uintptr_t callbackAddr);
};

#endif
