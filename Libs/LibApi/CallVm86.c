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

/**
 * Performs a Virtual 8086 BIOS interrupt call.
 *
 * @param interrupt:    number of the interrupt to fire
 * @param in:           input register values
 * @param out:          output register values
 * @return one of the {VM86_CALL_STATUS_*} status codes
 *
 * @security-level DRIVER
 */
Vm86CallStatus CallVm86(uint32_t interrupt, Vm86Registers *in, Vm86Registers *out)
{
	// prepare data
	SyscallCallVm86 data;
	data.interrupt = interrupt;
	data.in = *in;
	data.out = out;

	// perform call
	syscall(SYSCALL_CALL_VM86, (uint32_t) &data);

	// return status
	return data.status;
}
