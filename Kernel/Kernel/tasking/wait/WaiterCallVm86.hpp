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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_VIRTUAL_8086
#define EVA_MULTITASKING_WAIT_MANAGER_VIRTUAL_8086

#include "Api/Kernel.h"

#include <tasking/wait/waiter.hpp>

/**
 * waiter implementation for vm86 bios call
 */
class WaiterCallVm86 : public Waiter {
private:
    /**
     * internal data
     */
    SyscallCallVm86* data;                 // pointer to syscall data
    VM86Registers*   temporaryOut;         // pointer to vm86 registers descriptor
    uint32_t         virtual8086ProcessId; // process id of the task that keep waiting

public:
    /**
     * filled constructor
     *
     * @param _data:					pointer to do_syscall data
     * @param _temporaryOut:			pointer to vm86 registers descriptor
     * @param _virtual8086ProcessId:	process id of the task that keep waiting
     */
    WaiterCallVm86(SyscallCallVm86* _data,
                   VM86Registers*   _temporaryOut,
                   uint32_t         _virtual8086ProcessId)
        : data(_data), temporaryOut(_temporaryOut), virtual8086ProcessId(_virtual8086ProcessId) {
    }

    /**
     * implementation of check waiting method
     *
     * @param task:		the task that wait
     * @return true if task must keep waiting
     */
    virtual bool checkWaiting(Thread* task) {
        // get the thread descriptor
        Thread* vm86task = Tasking::getTaskById(virtual8086ProcessId);

        // check validity
        if ( !vm86task ) {
            // Task has finished, copy the values to the waiting data struct
            *(data->m_out_registers) = *temporaryOut;

            // Delete temporary out struct
            delete temporaryOut;

            return false;
        }

        // Keep waiting for Vm86 to finish
        else
            return true;
    }

    /**
     * @return the name of the waiter
     */
    virtual const char* debugName() {
        return "WaiterCallVm86";
    }
};

#endif
