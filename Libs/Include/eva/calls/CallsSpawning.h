/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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

#ifndef EVA_API_CALLS_SPAWNINGCALLS
#define EVA_API_CALLS_SPAWNINGCALLS

#include <eva/kernel.h>
#include <eva/ramdisk.h>
#include <eva/system.h>

__BEGIN_C

/**
 * @field path
 * 		absolute path of the binary
 *
 * @field securityLevel
 * 		target process security level
 *
 * @field spawnStatus
 * 		result of spawning
 */
typedef struct {
    char*         path;
    SecurityLevel securityLevel;

    RamdiskSpawnStatus spawnStatus;
} __attribute__((packed)) SyscallRamdiskSpawn;

/**
 * @field initialEntry
 * 		the initial thread entry
 *
 * @field userEntry
 * 		user-defined
 *
 * @field userData
 * 		user-defined
 *
 * @field status
 * 		result of thread creation
 */
typedef struct {
    void* initialEntry;
    void* userEntry;
    void* userData;
    char* threadName;

    CreateThreadStatus status;
    Tid                threadID;
} __attribute__((packed)) SyscallCreateThread;

/**
 * @field userEntry
 * 		the user entry
 *
 * @field userData
 * 		the user data
 */
typedef struct {
    void* userEntry;
    void* userData;
} __attribute__((packed)) SyscallGetThreadEntry;

/**
 * @field processObject
 * 		a handle to the created process, or 0 if the
 * 		creation has failed
 */
typedef struct {
    SecurityLevel             securityLevel;
    ProcessCreationIdentifier processObject;
} __attribute__((packed)) SyscallCreateEmptyProcess;

/**
 * @field processObject
 * 		handle to the target process
 *
 * @field targetSpaceVirtualAddress
 * 		virtual address in the target space to map the pages to
 *
 * @field numberOfPages
 * 		number of pages to map
 *
 * @field resultVirtualAddress
 * 		the virtual address of the mapped area in the current space
 */
typedef struct {
    ProcessCreationIdentifier processObject;
    uint32_t                  targetSpaceVirtualAddress;
    uint32_t                  numberOfPages;

    uint32_t resultVirtualAddress;
} __attribute__((packed)) SyscallCreatePagesInSpace;

/**
 * @field processObject
 * 		handle to the target process
 *
 * @field content
 * 		contents to copy to tls master copy
 *
 * @field copysize
 * 		number of bytes to copy from content
 *
 * @field totalsize
 * 		number of bytes the tls is in total (including bytes to zero)
 *
 * @field alignment
 * 		tls alignment
 *
 * @field result
 * 		whether mapping was successful
 */
typedef struct {
    ProcessCreationIdentifier processObject;
    uint8_t*                  content;
    uint32_t                  copysize;
    uint32_t                  totalsize;
    uint32_t                  alignment;

    uint8_t result;
} __attribute__((packed)) SyscallWriteTlsMasterForProcess;

/**
 * Used for process configuration on spawning.
 */
typedef struct {
    char* sourcePath;
} __attribute__((packed)) ProcessConfiguration;

/**
 * @field processObject
 * 		handle to the target process
 *
 * @field configuration
 * 		configuration content
 *
 * @field result
 * 		whether configuration was successful
 */
typedef struct {
    ProcessCreationIdentifier processObject;
    ProcessConfiguration      configuration;

    uint8_t result;
} __attribute__((packed)) SyscallConfigureProcess;

/**
 * @field eip
 * 		the start instruction address
 *
 * @field processObject
 * 		handle to the target process
 */
typedef struct {
    uint32_t                  eip;
    ProcessCreationIdentifier processObject;
} __attribute__((packed)) SyscallAttachCreatedProcess;

/**
 * @field processObject
 * 		handle to the target process
 */
typedef struct {
    ProcessCreationIdentifier processObject;
} __attribute__((packed)) SyscallCancelProcessCreation;

/**
 * @field processObject
 * 		handle to the target process
 *
 * @field resultId
 * 		the resulting process id
 */
typedef struct {
    ProcessCreationIdentifier processObject;

    Pid resultID;
} __attribute__((packed)) SyscallGetCreatedProcessID;

/**
 * @field processObject
 * 		handle to the target process
 *
 * @field arguments
 * 		source buffer, with a size of
 * 		{PROCESS_COMMAND_LINE_ARGUMENTS_BUFFER_LENGTH}
 */
typedef struct {
    ProcessCreationIdentifier processObject;
    char*                     arguments;
} __attribute__((packed)) SyscallCliArgsStore;

/**
 * @field buffer
 * 		target buffer, with a size of at least
 * 		{PROCESS_COMMAND_LINE_ARGUMENTS_BUFFER_LENGTH}
 */
typedef struct {
    char* buffer;
} __attribute__((packed)) SyscallCliArgsRelease;

__END_C

#endif
