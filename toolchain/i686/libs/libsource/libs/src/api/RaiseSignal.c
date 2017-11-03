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
#include <signal.h>

/**
 * Raises the <signal> in the <process>.
 *
 * @param process:  target process
 * @param signal:   signal number
 * @return one of the {RaiseSignalStatus} codes
 *
 * @security-level APPLICATION
 */
RaiseSignalStatus RaiseSignal(Pid process, int signal)
{
	// create data
	SyscallRaiseSignal data;
	data.process = process;
	data.signal = signal;

	// performs the call
	syscall(SYSCALL_RAISE_SIGNAL, (uint32_t) &data);

	// return the status
	return data.status;
}
