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
 * Registers the executing task for the given identifier.
 *
 * @param identifier:       the name to set
 * @param-opt selected:		dilled with the name selected by the kernel, empty if the provided is accepted
 * @return if it was successful true, if the identifier is taken false
 *
 * @security-level APPLICATION
 */
uint8_t TaskRegisterID(const char *newIdentifier)
{
	return TaskRegisterIDS(newIdentifier, NULL);
}

/**
 * Registers the executing task for the given identifier.
 *
 * @param identifier:       the name to set
 * @param-opt selected:		dilled with the name selected by the kernel, empty if the provided is accepted
 * @return if it was successful true, if the identifier is taken false
 *
 * @security-level APPLICATION
 */
uint8_t TaskRegisterIDS(const char *newIdentifier, char *selected)
{
	SyscallTaskIDRegister data;
	data.identifier = (char*) newIdentifier;

	// perform the call
	syscall(SYSCALL_REGISTER_THREAD_NAME, (uint32_t) &data);

	// check success
	if (!data.success && selected != NULL) memcopy(selected, data.selectedID, strlength(data.selectedID));

	// return success
	return data.success;
}
