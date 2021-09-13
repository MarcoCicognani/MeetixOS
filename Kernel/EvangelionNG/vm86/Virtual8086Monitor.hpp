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

#ifndef VM86_VIRTUAL8086_MONITOR
#define VM86_VIRTUAL8086_MONITOR

#include <system/ProcessorState.hpp>
#include <tasking/thread.hpp>

/**
 * flag macros
 */
#define EFLAG_IF    (1 << 9)
#define EFLAG_VM    (1 << 17)
#define VALID_FLAGS 0xDFF

/**
 * result codes for virtual monitor
 */
enum class VMResult : uint8_t
{
    SUCCESSFUL,
    UNHANDLED_OPCODE,
    FINISHED
};

/**
 * class to manage interface to the VM86
 */
class Virtual8086Monitor {
public:
    /**
     * handle a General Protection Fault exception and return a code
     *
     * @param thread:	the thread that have caused the exception
     */
    static VMResult handleGpf(Thread* thread);
};

#endif
