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

#include <Api/Syscalls/CallsData.h>
#include <BuildConfig.hpp>
#include <calls/SyscallHandler.hpp>
#include <EvangelionNG.hpp>
#include <executable/Elf32Loader.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/constants.hpp>
#include <memory/LowerHeap.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/TemporaryPagingUtil.hpp>
#include <ramdisk/ramdisk.hpp>
#include <system/interrupts/handling/InterruptRequestHandler.hpp>
#include <tasking/communication/MessageController.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>

/**
 * Define a macro to quickly check the validity of the call code provided
 */
#define IS_VALID_CODE(code) code < SYSCALL_COUNT

/**
 * Create the do_syscall table for handlers.
 * The do_syscall table is an array that contains the references
 * of the functions that handles the corresponding call code
 */
const SysCallHandler::CallHandler SysCallHandler::handlers[] = {
    [SYSCALL_THREAD_CREATE]          = &SysCallHandler::createThread,
    [SYSCALL_THREAD_GET_ENTRY]       = &SysCallHandler::getThreadEntry,
    [SYSCALL_THREAD_JOIN]            = &SysCallHandler::join,
    [SYSCALL_THREAD_YIELD]           = &SysCallHandler::yield,
    [SYSCALL_THREAD_GET_ID]          = &SysCallHandler::getTid,
    [SYSCALL_THREAD_GET_NAME]        = &SysCallHandler::getThreadName,
    [SYSCALL_THREAD_SET_NAME]        = &SysCallHandler::registerThreadName,
    [SYSCALL_THREAD_GET_ID_FOR_NAME] = &SysCallHandler::getTidByName,
    [SYSCALL_THREAD_SLEEP]           = &SysCallHandler::sleep,
    [SYSCALL_THREAD_EXIT]            = &SysCallHandler::exit,
    [SYSCALL_THREAD_KILL]            = &SysCallHandler::kill,
    [SYSCALL_EXIT_THREAD]            = &SysCallHandler::exitThread,

    [SYSCALL_PROCESS_FORK]                  = &SysCallHandler::fork,
    [SYSCALL_PROCESS_GET_ID]                = &SysCallHandler::getPid,
    [SYSCALL_PROCESS_GET_PARENT_ID]         = &SysCallHandler::getParentPid,
    [SYSCALL_PROCESS_GET_ID_FOR_THREAD_ID]  = &SysCallHandler::getPidForTid,
    [SYSCALL_PROCESS_CALL_VM86]             = &SysCallHandler::callVm86,
    [SYSCALL_PROCESS_STORE_CLI_ARGUMENTS]   = &SysCallHandler::cliArgsStore,
    [SYSCALL_PROCESS_RELEASE_CLI_ARGUMENTS] = &SysCallHandler::cliArgsRelease,

    [SYSCALL_SCHEDULER_GET_MILLISECONDS] = &SysCallHandler::millis,

    [SYSCALL_SIGNAL_RAISE]                = &SysCallHandler::raiseSignal,
    [SYSCALL_SIGNAL_REGISTER_HANDLER]     = &SysCallHandler::registerSignalHandler,
    [SYSCALL_SIGNAL_IRQ_REGISTER_HANDLER] = &SysCallHandler::registerIrqHandler,
    [SYSCALL_SIGNAL_IRQ_WAIT]             = &SysCallHandler::waitForIrq,
    [SYSCALL_SIGNAL_RESTORE_STATE]        = &SysCallHandler::restoreInterruptedState,

    [SYSCALL_LOCK_ATOMIC] = &SysCallHandler::atomicWait,

    [SYSCALL_PROCESS_CONFIGURE]            = &SysCallHandler::configureProcess,
    [SYSCALL_PROCESS_CREATE_EMPTY]         = &SysCallHandler::createEmptyProcess,
    [SYSCALL_CREATE_PAGES_IN_SPACE]        = &SysCallHandler::createPagesInSpace,
    [SYSCALL_GET_CREATED_PROCESS_ID]       = &SysCallHandler::getCreatedProcessId,
    [SYSCALL_ATTACH_CREATED_PROCESS]       = &SysCallHandler::attachCreatedProcess,
    [SYSCALL_CANCEL_PROCESS_CREATION]      = &SysCallHandler::cancelProcessCreation,
    [SYSCALL_WRITE_TLS_MASTER_FOR_PROCESS] = &SysCallHandler::writeTlsMasterForProcess,

    [SYSCALL_MESSAGE_SEND]    = &SysCallHandler::sendMessage,
    [SYSCALL_MESSAGE_RECEIVE] = &SysCallHandler::receiveMessage,

    [SYSCALL_MEMORY_SBRK]           = &SysCallHandler::sbrk,
    [SYSCALL_MEMORY_UNMAP]          = &SysCallHandler::unmap,
    [SYSCALL_MEMORY_SHARE]          = &SysCallHandler::shareMem,
    [SYSCALL_MEMORY_MAP_MMIO]       = &SysCallHandler::mapMmio,
    [SYSCALL_MEMORY_ALLOCATE]       = &SysCallHandler::allocMem,
    [SYSCALL_MEMORY_LOWER_FREE]     = &SysCallHandler::lowerFree,
    [SYSCALL_MEMORY_LOWER_ALLOCATE] = &SysCallHandler::lowerMalloc,

    [SYSCALL_LOG]              = &SysCallHandler::log,
    [SYSCALL_LOG_TOGGLE_VIDEO] = &SysCallHandler::setVideoLog,

    [SYSCALL_FS_OPEN]                   = &SysCallHandler::fsOpen,
    [SYSCALL_FS_READ]                   = &SysCallHandler::fsRead,
    [SYSCALL_FS_STAT]                   = &SysCallHandler::fsStat,
    [SYSCALL_FS_CLOSE]                  = &SysCallHandler::fsClose,
    [SYSCALL_FS_FSTAT]                  = &SysCallHandler::fsFstat,
    [SYSCALL_FS_WRITE]                  = &SysCallHandler::fsWrite,
    [SYSCALL_FS_LENGTH]                 = &SysCallHandler::fsLength,
    [SYSCALL_FS_PIPE]                   = &SysCallHandler::fsPipe,
    [SYSCALL_FS_SEEK]                   = &SysCallHandler::fsSeek,
    [SYSCALL_FS_TELL]                   = &SysCallHandler::fsTell,
    [SYSCALL_FS_CLONEFD]                = &SysCallHandler::fsClonefd,
    [SYSCALL_FS_OPEN_DIRECTORY]         = &SysCallHandler::fsOpenDirectory,
    [SYSCALL_FS_READ_DIRECTORY]         = &SysCallHandler::fsReadDirectory,
    [SYSCALL_FS_CREATE_NODE]            = &SysCallHandler::fsCreateNode,
    [SYSCALL_FS_REGISTER_AS_DELEGATE]   = &SysCallHandler::fsRegisterAsDelegate,
    [SYSCALL_FS_SET_TRANSACTION_STATUS] = &SysCallHandler::fsSetTransactionStatus,
    [SYSCALL_GET_EXECUTABLE_PATH]       = &SysCallHandler::getExecutablePath,
    [SYSCALL_GET_WORKING_DIRECTORY]     = &SysCallHandler::getWorkingDirectory,
    [SYSCALL_SET_WORKING_DIRECTORY]     = &SysCallHandler::setWorkingDirectory,
    [SYSCALL_ENAME]                     = &SysCallHandler::ename,
    [SYSCALL_SYSINFO]                   = &SysCallHandler::sysinfo,
    [SYSCALL_GET_PCI_COUNT]             = &SysCallHandler::getPciDeviceCount,
    [SYSCALL_GET_PCI_DEVICE]            = &SysCallHandler::getPciDevice,
    [SYSCALL_GET_THREAD_COUNT]          = &SysCallHandler::getThreadCount,
    [SYSCALL_GET_THREAD_IDS_LIST]       = &SysCallHandler::getThreadIDs,
    [SYSCALL_GET_THREAD_DESCRIPTOR]     = &SysCallHandler::getThreadDescriptor,
    [SYSCALL_GET_PROCESS_COUNT]         = &SysCallHandler::getProcessCount,
    [SYSCALL_GET_PROCESS_IDS_LIST]      = &SysCallHandler::getProcessIDs,
    [SYSCALL_GET_PROCESS_DESCRIPTOR]    = &SysCallHandler::getProcessDescriptor,

    [SYSCALL_SERVER_MANAGE]      = &SysCallHandler::serverManage,
    [SYSCALL_REGISTER_AS_SERVER] = &SysCallHandler::registerAsServer,
};

/**
 * The system call handler is called by the RequestHandler when a system call is executed.
 * The handler then checks which call was requested by taking it's number from
 * the callers EAX and redirects to the specific system call function.
 */
SYSCALL_HANDLER(handle) {
    // try to handle the call if have a valid code
    uint64_t index = SYSCALL_CODE(currentThread->cpuState);
    if ( IS_VALID_CODE(index) )
        return handlers[index](currentThread);

    // The system call could not be handled, this might mean that the
    // process was compiled for a deprecated/messed up API library and
    // is therefore not able to run well.
    logDebug("%! process %i tried to use non-existing do_syscall %i",
             "do_syscall",
             currentThread->id,
             index);

    // kill task for security
    currentThread->alive = false;

    // schedule to next process
    return Tasking::schedule();
}
