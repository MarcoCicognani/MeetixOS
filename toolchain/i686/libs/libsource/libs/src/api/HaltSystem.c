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

#include <eva/user.h>

/**
 * call spawner to do shutdown/reboot
 *
 * @param mode:		one of the {SPAWN_COMMAND} between REBOOT/SHUTDOWN
 */
void HaltMachine(int mode)
{
	// get spawner task identifier
	Tid spawnerTid = TaskGetID(SPAWNER_IDENTIFIER);
	if (spawnerTid == -1) return;

	// create transaction
	MessageTransaction tx = GetMessageTxId();

	// copy request contents
	SpawnerRequestHealtMachine request;
	request.header.command = mode;

	// send request to spawner
	SendMessageT(spawnerTid, &request, sizeof(SpawnerRequestHealtMachine), tx);
}

/**
 * reboot system
 */
void RebootSystem()
{
	HaltMachine(SPAWN_COMMAND_REBOOT_MACHINE);
}

/**
 * call spawner to shutdown the computer
 *
 * @security-level KERNEL
 */
void ShutdownSystem()
{
	HaltMachine(SPAWN_COMMAND_SHUTDOWN_MACHINE);
}
