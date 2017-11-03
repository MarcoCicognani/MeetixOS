/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]			                          *
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


#ifndef _EVA_EVAQUERY_
#define _EVA_EVAQUERY_

#include "eva/common.h"
#include "eva/kernel.h"

__BEGIN_C

/**
 * syscall uname data
 */
typedef struct
{
	char loaderName[10];	// loader name, hardcoded into the code and copyed on syscall
	size_t LversionMJ;		// version major
	size_t LversionMN;		// version minor
	uint8_t LversionPT;		// version patch

	char kernelName[13];	// kernel name, hardcoded into the code and copyed on syscall
	size_t versionMJ;		// version major
	size_t versionMN;		// version minor
	size_t versionSB;		// version sub
	uint8_t versionPT;		// version patch
} __attribute__((packed)) EvaName;

/**
 * syscall sysinfo data
 */
typedef struct
{
	uint32_t numberOfCore;		// number of cores
	char cpuVendor[13];			// cpu vendor
	uint32_t totRam;			// total ram in kb
	uint32_t freeRam;			// free ram in kb
} __attribute__((packed)) EvaSysInfo;

/**
 * PCI get count data
 */
typedef struct
{
	uint32_t count;		// number of pci devices avaible
}__attribute__((packed)) SyscallGetPciDeviceCount;

/**
 * PCI get device data
 */
typedef struct
{
	uint32_t position;		// position on the list
	uint8_t found;			// validity flag

	uint8_t slot;			// slot of the device
	uint8_t bus;			// bus used by the device
	uint8_t function;		// functionality

	uint16_t vendorId;		// vendor id
	uint16_t deviceId;		// device id

	uint8_t classCode;		// class
	uint8_t subclassCode;	// subclass
	uint8_t progIf;			// program
}__attribute__((packed)) PciDeviceHeader;

/**
 * Get process count data
 */
typedef struct
{
	uint32_t count;		// number of process in the system
}__attribute__((packed)) SyscallGetProcessCount;

/**
 * Get process id data
 */
typedef struct
{
	Pid *idBuffer;			// buffer with pids
	uint32_t idBufferSize;	// size of the buffer
	uint32_t filledIds;		// copied pids count
}__attribute__((packed)) SyscallGetProcessIDs;

/**
 * Get Thread count data
 */
typedef struct
{
	ThreadType type;	// the type of thread that want to get
	uint32_t count;		// number of process in the system
}__attribute__((packed)) SyscallGetThreadCount;

/**
 * Get Thread id data
 */
typedef struct
{
	Pid *idBuffer;			// buffer with pids
	ThreadType type;		// type of thread
	uint32_t idBufferSize;	// size of the buffer
	uint32_t filledIds;		// copied pids count
}__attribute__((packed)) SyscallGetThreadIDs;

/**
 * Thread descriptor
 */
typedef struct
{
	uint8_t found;			// validity flag

	Tid id;					// numberic id
	Pid parent;				// parent process id
	ThreadType type;		// type of thread
	char identifier[STRING_IDENTIFIER_MAX_LENGTH];	// thread/process name

	uint32_t threadUserStackSize;	// size of the user stack

	uint8_t priority;			// priority of the thread
	uint32_t waitCount;			// wait round count
	uint32_t executionTime;		// execution time
	uint32_t scheduledTimes;	// scheduled count
}__attribute__((packed)) ThreadDescriptor;

/**
 * Process descriptor
 */
typedef struct
{
	ThreadDescriptor main;				// infos about main Thread
	char sourcePath[PATH_MAX];			// the source path of the process
	uint32_t heapUsed;					// heap used in kb
	uint32_t imageSize;					// size of the image in kb
}__attribute__((packed)) ProcessDescriptor;

__END_C

#endif
