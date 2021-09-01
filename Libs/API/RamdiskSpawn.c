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
 * Spawns a binary from the ramdisk.
 *
 * @param path:             ramdisk-relative path to the binary
 * @param securityLevel:    the security level to set for the spawned process
 * @return one the {RamdiskSpawnStatus} codes
 *
 * @param security-level KERNEL
 */
RamdiskSpawnStatus RamdiskSpawn(const char *path, SecurityLevel securityLevel)
{
	// create data
	SyscallRamdiskSpawn spawnData;
	spawnData.path = (char*) path;
	spawnData.securityLevel = securityLevel;

	// performs call
	syscall(SYSCALL_RAMDISK_SPAWN, (uint32_t) & spawnData);

	// return status
	return spawnData.spawnStatus;
}
