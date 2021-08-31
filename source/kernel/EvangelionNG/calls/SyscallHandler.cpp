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
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "eva/calls/calls.h"
#include <calls/SyscallHandler.hpp>

#include <EvangelionNG.hpp>
#include <BuildConfig.hpp>
#include <ramdisk/ramdisk.hpp>
#include <executable/Elf32Loader.hpp>
#include <logger/logger.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/communication/MessageController.hpp>
#include <system/interrupts/handling/InterruptRequestHandler.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/TemporaryPagingUtil.hpp>
#include <memory/constants.hpp>
#include <memory/LowerHeap.hpp>

/**
 * Define a macro to quickly check the validity of the call code provided
 */
#define IS_VALID_CODE(code) code < SYSCALL_COUNT

/**
 * Create the syscall table for handlers.
 * The syscall table is an array that contains the references
 * of the functions that handles the corresponding call code
 */
const SysCallHandler::CallHandler SysCallHandler::handlers[] = {
	// Process / Threads interaction
	[SYSCALL_JOIN]  					   =		&SysCallHandler::join,
	[SYSCALL_YIELD] 					   = 		&SysCallHandler::yield,
	[SYSCALL_CALL_VM86] 				   = 		&SysCallHandler::callVm86,
	[SYSCALL_GET_THREAD_ID] 	 		   = 		&SysCallHandler::getTid,
	[SYSCALL_GET_PROCESS_ID] 			   = 		&SysCallHandler::getPid,
	[SYSCALL_GET_THREAD_NAME] 		  	   = 		&SysCallHandler::getThreadName,
	[SYSCALL_GET_MILLISECONDS] 			   = 		&SysCallHandler::millis,
	[SYSCALL_REGISTER_THREAD_NAME] 	       = 		&SysCallHandler::registerThreadName,
	[SYSCALL_GET_PARENT_PROCESS_ID] 	   =		&SysCallHandler::getParentPid,
	[SYSCALL_GET_THREAD_ID_FOR_NAME] 	   = 		&SysCallHandler::getTidByName,
	[SYSCALL_GET_PROCESS_ID_FOR_THREAD_ID] = 		&SysCallHandler::getPidForTid,

	// Servers controller
	[SYSCALL_SERVER_MANAGE]				   =		&SysCallHandler::serverManage,
	[SYSCALL_REGISTER_AS_SERVER]		   =		&SysCallHandler::registerAsServer,

	// Process/Thread termination
	[SYSCALL_EXIT]  					   = 		&SysCallHandler::exit,
	[SYSCALL_KILL] 						   =		&SysCallHandler::kill,
	[SYSCALL_EXIT_THREAD]           	   =		&SysCallHandler::exitThread,

	// Signals and interrupts managing
	[SYSCALL_RAISE_SIGNAL] 				   =		&SysCallHandler::raiseSignal,
	[SYSCALL_REGISTER_IRQ_HANDLER] 		   =		&SysCallHandler::registerIrqHandler,
	[SYSCALL_REGISTER_SIGNAL_HANDLER] 	   =		&SysCallHandler::registerSignalHandler,
	[SYSCALL_RESTORE_INTERRUPTED_STATE]    =		&SysCallHandler::restoreInterruptedState,

	// Thread waiting
	[SYSCALL_SLEEP] 					   =		&SysCallHandler::sleep,
	[SYSCALL_ATOMIC_LOCK] 				   =		&SysCallHandler::atomicWait,
	[SYSCALL_WAIT_FOR_IRQ] 				   =		&SysCallHandler::waitForIrq,

	// Process arguments managing
	[SYSCALL_STORE_CLI_ARGUMENTS] 		   =		&SysCallHandler::cliArgsStore,
	[SYSCALL_RELEASE_CLI_ARGUMENTS] 	   =		&SysCallHandler::cliArgsRelease,

	// Process / Threads creation
	[SYSCALL_FORK]  					   =		&SysCallHandler::fork,
	[SYSCALL_RAMDISK_SPAWN]    	   		   = 		&SysCallHandler::ramdiskSpawn,
	[SYSCALL_CREATE_THREAD]    	   		   = 		&SysCallHandler::createThread,
	[SYSCALL_GET_THREAD_ENTRY] 	   		   = 		&SysCallHandler::getThreadEntry,
	[SYSCALL_CONFIGURE_PROCESS] 		   = 		&SysCallHandler::configureProcess,
	[SYSCALL_CREATE_EMPTY_PROCESS] 		   = 		&SysCallHandler::createEmptyProcess,
	[SYSCALL_CREATE_PAGES_IN_SPACE] 	   = 		&SysCallHandler::createPagesInSpace,
	[SYSCALL_GET_CREATED_PROCESS_ID] 	   = 		&SysCallHandler::getCreatedProcessId,
	[SYSCALL_ATTACH_CREATED_PROCESS] 	   = 		&SysCallHandler::attachCreatedProcess,
	[SYSCALL_CANCEL_PROCESS_CREATION] 	   = 		&SysCallHandler::cancelProcessCreation,
	[SYSCALL_WRITE_TLS_MASTER_FOR_PROCESS] = 		&SysCallHandler::writeTlsMasterForProcess,

	// Thread messaging
	[SYSCALL_MESSAGE_SEND] 				   = 		&SysCallHandler::sendMessage,
	[SYSCALL_MESSAGE_RECEIVE] 			   = 		&SysCallHandler::receiveMessage,

	// Memory management
	[SYSCALL_SBRK] 						   = 		&SysCallHandler::sbrk,
	[SYSCALL_UNMAP] 					   = 		&SysCallHandler::unmap,
	[SYSCALL_SHARE_MEMORY]  			   = 		&SysCallHandler::shareMem,
	[SYSCALL_MAP_MMIO_AREA] 			   = 		&SysCallHandler::mapMmio,
	[SYSCALL_ALLOCATE_MEMORY] 			   = 		&SysCallHandler::allocMem,
	[SYSCALL_LOWER_MEMORY_FREE] 		   = 		&SysCallHandler::lowerFree,
	[SYSCALL_LOWER_MEMORY_ALLOCATE] 	   = 		&SysCallHandler::lowerMalloc,

	// Log management
	[SYSCALL_LOG]						   =		&SysCallHandler::log,
	[SYSCALL_SET_VIDEO_LOG] 			   =		&SysCallHandler::setVideoLog,

	// Direct ramdisk access
	[SYSCALL_RAMDISK_FIND] 				   = 		&SysCallHandler::ramdiskFind,
	[SYSCALL_RAMDISK_INFO] 		  		   = 		&SysCallHandler::ramdiskInfo,
	[SYSCALL_RAMDISK_READ] 		  		   = 		&SysCallHandler::ramdiskRead,
	[SYSCALL_RAMDISK_CHILD_AT] 			   = 		&SysCallHandler::ramdiskChildAt,
	[SYSCALL_RAMDISK_FIND_CHILD] 		   = 		&SysCallHandler::ramdiskFindChild,
	[SYSCALL_RAMDISK_CHILD_COUNT] 		   = 		&SysCallHandler::ramdiskChildCount,

	// File operations
	[SYSCALL_FS_OPEN]  					   = 		&SysCallHandler::fsOpen,
	[SYSCALL_FS_READ]  					   = 		&SysCallHandler::fsRead,
	[SYSCALL_FS_STAT]  	 				   = 		&SysCallHandler::fsStat,
	[SYSCALL_FS_CLOSE] 					   = 		&SysCallHandler::fsClose,
	[SYSCALL_FS_FSTAT] 	 				   = 		&SysCallHandler::fsFstat,
	[SYSCALL_FS_WRITE] 					   = 		&SysCallHandler::fsWrite,
	[SYSCALL_FS_LENGTH] 				   = 		&SysCallHandler::fsLength,

	// File System operations
	[SYSCALL_FS_PIPE] 					   = 		&SysCallHandler::fsPipe,
	[SYSCALL_FS_SEEK] 					   = 		&SysCallHandler::fsSeek,
	[SYSCALL_FS_TELL] 					   = 		&SysCallHandler::fsTell,
	[SYSCALL_FS_CLONEFD] 				   = 		&SysCallHandler::fsClonefd,
	[SYSCALL_FS_OPEN_DIRECTORY] 		   = 		&SysCallHandler::fsOpenDirectory,
	[SYSCALL_FS_READ_DIRECTORY] 		   = 		&SysCallHandler::fsReadDirectory,

	// Userspace File System driver
	[SYSCALL_FS_CREATE_NODE]    		   = 		&SysCallHandler::fsCreateNode,
	[SYSCALL_FS_REGISTER_AS_DELEGATE] 	   = 		&SysCallHandler::fsRegisterAsDelegate,
	[SYSCALL_FS_SET_TRANSACTION_STATUS]    = 		&SysCallHandler::fsSetTransactionStatus,

	// Path utils
	[SYSCALL_GET_EXECUTABLE_PATH]   	   =		&SysCallHandler::getExecutablePath,
	[SYSCALL_GET_WORKING_DIRECTORY] 	   =		&SysCallHandler::getWorkingDirectory,
	[SYSCALL_SET_WORKING_DIRECTORY] 	   =		&SysCallHandler::setWorkingDirectory,

	// System Infos
	[SYSCALL_ENAME]   					   = 		&SysCallHandler::ename,
	[SYSCALL_SYSINFO] 					   = 		&SysCallHandler::sysinfo,
	[SYSCALL_GET_PCI_COUNT]				   =		&SysCallHandler::getPciDeviceCount,
	[SYSCALL_GET_PCI_DEVICE]			   =		&SysCallHandler::getPciDevice,

	// Thread Infos
	[SYSCALL_GET_THREAD_COUNT]			   =		&SysCallHandler::getThreadCount,
	[SYSCALL_GET_THREAD_IDS_LIST]		   =		&SysCallHandler::getThreadIDs,
	[SYSCALL_GET_THREAD_DESCRIPTOR]		   =		&SysCallHandler::getThreadDescriptor,

	// Process Infos
	[SYSCALL_GET_PROCESS_COUNT]			   =		&SysCallHandler::getProcessCount,
	[SYSCALL_GET_PROCESS_IDS_LIST]		   =		&SysCallHandler::getProcessIDs,
	[SYSCALL_GET_PROCESS_DESCRIPTOR]       =		&SysCallHandler::getProcessDescriptor,
};

/**
 * The system call handler is called by the RequestHandler when a system call is executed.
 * The handler then checks which call was requested by taking it's number from
 * the callers EAX and redirects to the specific system call function.
 */
SYSCALL_HANDLER(handle)
{
	// try to handle the call if have a valid code
	uint64_t index = SYSCALL_CODE(currentThread->cpuState);
	if (IS_VALID_CODE(index)) return handlers[index](currentThread);

	// The system call could not be handled, this might mean that the
	// process was compiled for a deprecated/messed up API library and
	// is therefore not able to run well.
	logDebug("%! process %i tried to use non-existing syscall %i", "syscall", currentThread->id, index);

	// kill task for security
	currentThread->alive = false;

	// schedule to next process
	return Tasking::schedule();
}
