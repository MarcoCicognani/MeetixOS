/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "__internal.h"
#include "eva/user.h"

/**
 * Registers the <handler> routine as the handler for the <irq>.
 *
 * @param signal:       signal to handle
 * @param handler:      handler routine to call
 * @return:             previously registered handler
 *
 * @security-level DRIVER
 */
void* RegisterSignalHandler(int signal, void* handler) {
    // prepare data
    SyscallRegisterSignalHandler data;
    data.signal   = signal;
    data.handler  = (uintptr_t)handler;
    data.callback = (uintptr_t)__RestoreInterruptedStateCallback;

    // performs the call
    syscall(SYSCALL_REGISTER_SIGNAL_HANDLER, (uint32_t)&data);

    // return the previous registered handler
    return (void*)data.previousHandler;
}
