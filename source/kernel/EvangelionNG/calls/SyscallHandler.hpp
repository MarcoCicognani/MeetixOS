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

#ifndef EVA_SYSTEM_CALLS
#define EVA_SYSTEM_CALLS

#include <utils/string.hpp>
#include <memory/paging.hpp>
#include <memory/memory.hpp>
#include <system/ProcessorState.hpp>
#include <tasking/thread.hpp>

// Macros for call code and data
#define SYSCALL_CODE(state)				state->eax
#define SYSCALL_DATA(state)				state->ebx

// Call handler macro
#define SYSCALL_HANDLER(callName)		Thread *SysCallHandler::callName(Thread *currentThread)

/**
 * The system call handler is called by the RequestHandler when a system call is executed.
 * The handler then checks which call was requested by taking it's number from
 * the callers EAX and redirects to the specific system call function.
 */
class SysCallHandler
{
public:
	// Main Handler of syscall
	static Thread *handle(Thread *state);

private:
	/**
	 * Storage for syscalls functions
	 */
	typedef Thread *(*CallHandler)(Thread *state);
	static const CallHandler handlers[];

	/**
	 * Process interaction
	 */
	static Thread *join(Thread *state);
 	static Thread *yield(Thread *state);
 	static Thread *callVm86(Thread *state);
 	static Thread *getTid(Thread *state);
 	static Thread *getPid(Thread *state);
	static Thread *getThreadName(Thread *state);
 	static Thread *millis(Thread *state);
	static Thread *registerThreadName(Thread *state);
 	static Thread *getParentPid(Thread *state);
 	static Thread *getTidByName(Thread *state);
 	static Thread *getPidForTid(Thread *state);

	/**
	 * Servers controller
	 */
	static Thread *serverManage(Thread *state);
	static Thread *registerAsServer(Thread *state);

	/**
	 * Process / Thread termination
	 */
	static Thread *exit(Thread *state);
 	static Thread *kill(Thread *state);
 	static Thread *exitThread(Thread *state);

	/**
	 * Signals and interrupts
	 */
	static Thread *raiseSignal(Thread *state);
	static Thread *registerIrqHandler(Thread *state);
	static Thread *registerSignalHandler(Thread *state);
	static Thread *restoreInterruptedState(Thread *state);

	/**
	 * Wait
	 */
	static Thread *sleep(Thread *state);
	static Thread *atomicWait(Thread *state);
	static Thread *waitForIrq(Thread *state);

	/**
	 * Arguments
	 */
	static Thread *cliArgsStore(Thread *state);
	static Thread *cliArgsRelease(Thread *state);

	/**
	 * Process / Threads creations
	 */
	static Thread *fork(Thread *state);
 	static Thread *ramdiskSpawn(Thread *state);
 	static Thread *createThread(Thread *state);
 	static Thread *getThreadEntry(Thread *state);
 	static Thread *configureProcess(Thread *state);
 	static Thread *createEmptyProcess(Thread *state);
 	static Thread *createPagesInSpace(Thread *state);
 	static Thread *getCreatedProcessId(Thread *state);
 	static Thread *attachCreatedProcess(Thread *state);
 	static Thread *cancelProcessCreation(Thread *state);
 	static Thread *writeTlsMasterForProcess(Thread *state);

	/**
	 * Messaging
	 */
	static Thread *sendMessage(Thread *state);
 	static Thread *receiveMessage(Thread *state);

	/**
	 * Memory management
	 */
	static Thread *sbrk(Thread *state);
    static Thread *unmap(Thread *state);
    static Thread *shareMem(Thread *state);
    static Thread *mapMmio(Thread *state);
    static Thread *allocMem(Thread *state);
    static Thread *lowerFree(Thread *state);
    static Thread *lowerMalloc(Thread *state);

	/**
	 * Serial Log
	 */
	static Thread *log(Thread *state);
 	static Thread *setVideoLog(Thread *state);

	/**
	 * Direct ramdisk access
	 */
	static Thread *ramdiskFind(Thread *state);
	static Thread *ramdiskInfo(Thread *state);
	static Thread *ramdiskRead(Thread *state);
	static Thread *ramdiskChildAt(Thread *state);
	static Thread *ramdiskFindChild(Thread *state);
	static Thread *ramdiskChildCount(Thread *state);

	/**
	 * File operations
	 */
	static Thread *fsOpen(Thread *state);
	static Thread *fsRead(Thread *state);
	static Thread *fsStat(Thread *state);
	static Thread *fsClose(Thread *state);
	static Thread *fsFstat(Thread *state);
	static Thread *fsWrite(Thread *state);
	static Thread *fsLength(Thread *state);

	/**
	 * File system operations
	 */
	static Thread *fsPipe(Thread *state);
 	static Thread *fsSeek(Thread *state);
 	static Thread *fsTell(Thread *state);
 	static Thread *fsClonefd(Thread *state);
 	static Thread *fsOpenDirectory(Thread *state);
 	static Thread *fsReadDirectory(Thread *state);

	/**
	 * File system driver
	 */
	static Thread *fsCreateNode(Thread *state);
 	static Thread *fsRegisterAsDelegate(Thread *state);
 	static Thread *fsSetTransactionStatus(Thread *state);

	/**
	 * Path utils
	 */
	static Thread *getExecutablePath(Thread *state);
	static Thread *getWorkingDirectory(Thread *state);
	static Thread *setWorkingDirectory(Thread *state);

	/**
	 * System Infos
	 */
	static Thread *ename(Thread *state);
	static Thread *sysinfo(Thread *state);
	static Thread *getPciDeviceCount(Thread *state);
	static Thread *getPciDevice(Thread *state);

	/**
	 * Thread infos
	 */
	static Thread *getThreadCount(Thread *state);
	static Thread *getThreadIDs(Thread *state);
	static Thread *getThreadDescriptor(Thread *state);

	/**
	 * Process infos
	 */
	static Thread *getProcessCount(Thread *state);
	static Thread *getProcessIDs(Thread *state);
	static Thread *getProcessDescriptor(Thread *state);
};

#endif
