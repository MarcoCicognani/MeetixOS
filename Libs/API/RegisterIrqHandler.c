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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include "eva/user.h"
#include "__internal.h"

/**
 * Registers the <handler> routine as the handler for the <irq>.
 *
 * @param irq:          IRQ number to handle
 * @param handler:      handler routine to call
 * @return one of the {RegisterIrqHandlerStatus} codes
 *
 * @security-level DRIVER
 */
RegisterIrqHandlerStatus RegisterIrqHandler(uint8_t irq, void (*handler)(uint8_t))
{
	// prepare the data
	SyscallRegisterIrqHandler data;
	data.irq = irq;
	data.handler = (uintptr_t) handler;
	data.callback = (uintptr_t) __RestoreInterruptedStateCallback;

	// performs the call
	syscall(SYSCALL_REGISTER_IRQ_HANDLER, (uint32_t) &data);

	// return the status
	return data.status;
}
