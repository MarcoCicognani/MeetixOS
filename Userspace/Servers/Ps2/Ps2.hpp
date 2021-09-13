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

#ifndef __PS2_DRIVER__
#define __PS2_DRIVER__

#include <stdint.h>

/**
 * The operation mode declares whether the IRQs shall be handled by polling the kernel
 * or by registering a handler that is called upon interrupt receival.
 *
 * The polling mode might create higher latency due to the kernel being involved more,
 * but is more stable as the interrupt is handled on kernel side.
 *
 * Default operation mode is the IRQ-triggered mode.
 */
#define DRIVER_OPERATION_MODE_IRQ_TRIGGERED 0
#define DRIVER_OPERATION_MODE_POLLING       1

#define DRIVER_OPERATION_MODE DRIVER_OPERATION_MODE_IRQ_TRIGGERED

/**
 * Operation mode specific header files
 */
#if DRIVER_OPERATION_MODE == DRIVER_OPERATION_MODE_POLLING
#    include "Ps2Polling.hpp"
#elif DRIVER_OPERATION_MODE == DRIVER_OPERATION_MODE_IRQ_TRIGGERED
#    include "Ps2IrqTriggered.hpp"
#endif

/**
 * Counter for the absolute number of packages that where received from the devices.
 */
extern uint64_t packetsCount;

/**
 * Used when waiting for a buffer to determine whether the driver
 * shall wait for the input or output buffer of the device.
 */
enum Ps2Buffer_t
{
    PS2_OUT,
    PS2_IN
};

/**
 * Initializes the mouse by resetting it to the defaults and enabling it. This involves
 * a series of commands that are written to the device.
 */
void initializeMouse();

/**
 * Waits for the PS2 input or output buffer by querying the flags in the status byte.
 */
void waitForBuffer(Ps2Buffer_t mode);

/**
 * Writes a byte to the mouse.
 *
 * @param b
 * 		byte to write
 */
void writeToMouse(uint8_t b);

/**
 * Registers operation mode specific handling logic.
 */
void registerOperationMode();

/**
 * Handles an incoming byte from the mouse.
 *
 * @param b
 * 		value received from the mouse
 */
void handleMouseData(uint8_t b);

/**
 * Handles an incoming byte from the keyboard.
 *
 * @param b
 * 		value received from the keyboard
 */
void handleKeyboardData(uint8_t b);

#endif
