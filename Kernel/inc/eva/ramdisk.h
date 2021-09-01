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
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * */

#ifndef __EVA_SYS_RAMDISK__
#define __EVA_SYS_RAMDISK__

#include "eva/common.h"

__BEGIN_C

// ramdisk spawning status codes
typedef uint8_t RamdiskSpawnStatus;
#define RAMDISK_SPAWN_STATUS_SUCCESSFUL				1
#define RAMDISK_SPAWN_STATUS_FAILED_NOT_FOUND		2
#define RAMDISK_SPAWN_STATUS_FAILED_NOT_VALID		3
#define RAMDISK_SPAWN_STATUS_FAILED_NOT_PERMITTED	4

/**
 * Maximum length of paths within the ramdisk
 */
#define RAMDISK_MAXIMUM_PATH_LENGTH				512

// types of ramdisk entries
typedef int RamdiskEntryType;
#define RAMDISK_ENTRY_TYPE_UNKNOWN		-1
#define RAMDISK_ENTRY_TYPE_FOLDER		0
#define RAMDISK_ENTRY_TYPE_FILE			1

/**
 * Ramdisk entry information struct used within system calls
 */
typedef struct 
{
	RamdiskEntryType type;
	char name[512];
	unsigned int length;
} RamdiskEntryInfo;

__END_C

#endif
