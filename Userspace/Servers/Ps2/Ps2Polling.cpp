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

#include "Ps2.hpp"

#include <eva.h>
#include <tasking/lock.hpp>
#include <utils/utils.hpp>

#if DRIVER_OPERATION_MODE == DRIVER_OPERATION_MODE_POLLING

/**
 *
 */
void registerOperationMode() {
    Utils::log("starting poll thread");
    CreateThread((void*)pollingThread);
}

/**
 *
 */
void pollingThread() {
    while ( true ) {
        poll();
    }
}

/**
 *
 */
void poll() {
    static Lock pollLock;
    pollLock.lock();

    uint8_t status;
    while ( ((status = Utils::Cpu::inportByte(0x64)) & 1) != 0 ) {
        uint8_t value = Utils::Cpu::inportByte(0x60);

        bool fromKeyboard = ((status & (1 << 5)) == 0);

        if ( fromKeyboard )
            handleKeyboardData(value);
        else
            handleMouseData(value);

        ++packetsCount;
    }

    pollLock.unlock();
}

#endif
