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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef EVA_KERNEL_SYSTEM_CALLS
#define EVA_KERNEL_SYSTEM_CALLS

#include <eva/calls/calls.h>
#include <eva/common.h>
#include <eva/fs.h>
#include <eva/info.h>
#include <eva/ipc.h>
#include <eva/kernel.h>
#include <eva/ramdisk.h>
#include <eva/system.h>
#include <eva/types.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

__BEGIN_C

/**
 * Performs an atomic wait. If the atom is true, the executing task must
 * wait until the task that owns the atom has finished its work and sets
 * it to false. Once the atom is false, it is set to true and the function
 * returns.
 *
 * @param atom:         the atom to use
 * @param-opt atom2:    the second atom to use
 *
 * @security-level APPLICATION
 */
void AtomicLock(uint8_t* atom);
void AtomicLockDual(uint8_t* atom1, uint8_t* atom2);

/**
 * Trys to perform atomic wait. If the lock is already locked, the function
 * returns 0. Otherwise, the lock is set as in {AtomicLock} and the
 * function returns 1.
 *
 * @param atom:         the atom to use
 * @param-opt atom2:    the second atom to use
 *
 * @security-level APPLICATION
 */
uint8_t AtomicTryLock(uint8_t* atom);
uint8_t AtomicTryLockDual(uint8_t* atom1, uint8_t* atom2);

/**
 * Performs an atomic block. If the atom is true, the executing task must
 * wait until the task that owns the atom has finished its work and sets
 * it to false. Different from the {AtomicLock}, the atom is not changed.
 *
 * @param atom:         the atom to use
 * @param-opt atom2:    the second atom to use
 *
 * @security-level APPLICATION
 */
void AtomicBlock(uint8_t* atom);
void AtomicBlockDual(uint8_t* atom1, uint8_t* atom2);

/**
 * Spawns a program binary.
 *
 * @param path:             absolute path of the executable
 * @param args:             unparsed arguments
 * @param workdir:          working directory for the execution
 * @param securityLevel:    security level to spawn the process to
 * @param-opt pid:          is filled with the process id
 * @param-opt outStdio:     is filled with stdio file descriptors, 0 is write end of stdin, 1 is
 * read end of stdout, 2 is read end of stderr
 * @param-opt inStdio:      if supplied, the given descriptors which are valid for the executing
 * process are used as the stdin/out/err for the spawned process; an entry might be -1 to be ignored
 * and default behaviour being applied
 * @return one of the {SpawnStatus} codes
 *
 * @security-level APPLICATION
 */
SpawnStatus
Spawn(const char* path, const char* args, const char* workdir, SecurityLevel securityLevel);
SpawnStatus SpawnP(const char*   path,
                   const char*   args,
                   const char*   workdir,
                   SecurityLevel securityLevel,
                   Pid*          pid);
SpawnStatus SpawnPO(const char*   path,
                    const char*   args,
                    const char*   workdir,
                    SecurityLevel securityLevel,
                    Pid*          pid,
                    File_t        outStdio[3]);
SpawnStatus SpawnPOI(const char*   path,
                     const char*   args,
                     const char*   workdir,
                     SecurityLevel securityLevel,
                     Pid*          pid,
                     File_t        outStdio[3],
                     File_t        inStdio[3]);

/**
 * Register the current process as a kernel
 * server and return one of the {RegisterAsServerStatus} codes.
 * Update the security level of the process and permits to increase it since the
 * {SECURITY_LEVEL_SERVER}
 *
 * @param serverIdentifier:		the name of the server
 * @param slevel:				the new security level for the process
 * @return one of the {RegisterAsServerStatus} codes
 */
RegisterAsServerStatus RegisterAsServer(const char* serverIdentifier, SecurityLevel slevel);

/**
 * Provides a standard interface to communicate with servers,
 * first the call send a message to the server identified by <serverIdentifier>,
 * if it exists, then wait with a receive message the response of the server, that provides data by
 * the <data> parameter
 *
 * @param serverIdentifier:			the name of the server
 * @param command:					a command to send to the server
 * @param data:						a pointer the data that process the server
 * called
 * @param length:					the length of the data buffer
 * @return one of the {ServerManageStatus} codes.
 */
ServerManageStatus ServerManage(const char*         serverIdentifier,
                                ServerManageCommand command,
                                void*               data,
                                uint32_t            length);

/**
 * call spawner to reboot the computer
 *
 * @security-level KERNEL
 */
void RebootSystem();

/**
 * call spawner to shutdown the computer
 *
 * @security-level KERNEL
 */
void ShutdownSystem();

/**
 * Kills a process.
 *
 * @param pid:      the process id
 * @return one of the {KillStatus} codes
 *
 * @security-level APPLICATION
 */
KillStatus Kill(Pid pid);

/**
 * Prints a message to the log.
 *
 * @param message:      the message to log
 *
 * @security-level APPLICATION
 */
void Log(const char* message);

/**
 * Performs the software interrupt necessary for the system call passing the
 * given data (usually a pointer to a call struct).
 *
 * @param call:     the call to execute
 * @param data:     the data to pass
 *
 * Clobber flags (more: https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html):
 * - The "cc" clobber indicates that the assembler code modifies the FLAGS register
 * - The "memory" clobber tells the compiler that the assembly code performs memory reads
 *   or writes to items other than those listed in the input and output operands. Causes
 *   GCC to assume that anything in memory could have changed (the system call could do so).
 *
 * @security-level APPLICATION
 */
void syscall(uint32_t call, uint32_t data);

/**
 * Opens a file.
 *
 * @param path:            the path to the file
 * @param-opt flags:       the flags for open mode
 * @param-opt mode:        the mode that kernel use to open the file
 * @param-opt outStatus:   filled with one of the {FsOpenStatus} codes
 * @return the file descriptor for the opened file
 *
 * @security-level APPLICATION
 */
File_t Open(const char* path);
File_t OpenF(const char* path, int32_t flags);
File_t OpenFS(const char* path, int32_t flags, FsOpenStatus* outStatus);
File_t OpenFMS(const char* path, int32_t flags, int32_t mode, FsOpenStatus* outStatus);

/**
 * Closes a file.
 *
 * @param fd:   the file descriptor to close
 * @return one of the {FsCloseStatus} codes
 *
 * @security-level APPLICATION
 */
FsCloseStatus Close(File_t fd);

/**
 * Retrieves the length of a file in bytes.
 *
 * @param fd:                   the file descriptor
 * @param-opt outStatus:        is filled with the status code
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t Length(File_t fd);
int64_t LengthS(File_t fd, FsLengthStatus* outStatus);

/**
 * Opens a directory.
 *
 * @param path:             path of the directory
 * @param-opt outStatus:    is filled with the out status
 * @return pointer to a directory iterator, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryIterator* OpenDirectory(const char* path);
FsDirectoryIterator* OpenDirectoryS(const char* path, FsOpenDirectoryStatus* outStatus);

/**
 * Reads the next entry of the directory.
 *
 * @param iterator:         the directory iterator
 * @param-opt outStatus:    is filled with the status code
 * @return a directory entry structure, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryEntry* ReadDirectory(FsDirectoryIterator* iterator);
FsDirectoryEntry* ReadDirectoryS(FsDirectoryIterator* iterator, FsReadDirectoryStatus* outStatus);

/**
 * Closes a directory.
 *
 * @param iterator:     the directory iterator
 *
 * @security-level APPLICATION
 */
void CloseDirectory(FsDirectoryIterator* iterator);

/**
 * Retrieves the length of a file in bytes.
 *
 * @param path:                     path of the file
 * @param-opt followSymlinks:       whether to follow symbolic links or not
 * @param-opt outStatus:            is filled with the status code
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t Flength(const char* path);
int64_t FlengthS(const char* path, uint8_t followSymlinks);
int64_t FlengthSS(const char* path, uint8_t followSymlinks, FsLengthStatus* outStatus);

/**
 * Repositions the offset within a file.
 *
 * @param fd:               the file descriptor
 * @param off:              the offset
 * @param whence:           one of the {FsSeekMode} constants
 * @param-opt outStatus:    is filled with the status
 * @return if successful returns the new absolute offset, otherwise -1
 *
 * @security-level APPLICATION
 */
int64_t Seek(File_t fd, int64_t off, FsSeekMode mode);
int64_t SeekS(File_t fd, int64_t off, FsSeekMode mode, FsSeekStatus* outStatus);

/**
 * Retrieves the current offset within a file.
 *
 * @param fd:               the file descriptor
 * @param-opt outStatus:    is filled with the status
 * @return if successful returns the current absolute offset, otherwise -1
 *
 * @security-level APPLICATION
 */
int64_t Tell(File_t fd);
int64_t TellS(File_t fd, FsTellStatus* outStatus);

/**
 * Sets the working directory for the current process.
 *
 * @param path:         buffer of at least {PATH_MAX} bytes size
 * 		                containing the new working directory
 * @param process:      process to set working directory for during spawning
 * @return one of the {SetWorkingDirectoryStatus} codes
 *
 * @security-level APPLICATION if no process given, otherwise KERNEL
 */
SetWorkingDirectoryStatus SetWorkingDirectory(const char* path);
SetWorkingDirectoryStatus SetWorkingDirectoryP(const char* path, ProcessCreationIdentifier process);

/**
 * Retrieves the working directory for the current process.
 *
 * @param path:         buffer of at least <maxlen> or {PATH_MAX} bytes size
 * @param-opt maxlen:   length of the buffer in bytes
 * @return whether the action was successful
 *
 * @security-level APPLICATION
 */
GetWorkingDirectoryStatus GetWorkingDirectory(char* buffer);
GetWorkingDirectoryStatus GetWorkingDirectoryL(char* buffer, size_t maxlen);

/**
 * Retrieves the directory of the executable when available, otherwise an empty
 * string is written to the buffer.
 *
 * @param path:     buffer of at least {PATH_MAX} bytes size
 *
 * @security-level APPLICATION
 */
void GetExecutablePath(char* buffer);

/**
 * Reads bytes from the file to the buffer.
 *
 * @param fd:               the file descriptor
 * @param buffer:           the target buffer
 * @param length:           the length in bytes
 * @param-opt outStatus:    filled with one of the {FsReadStatus} codes
 * @return if the read was successful the length of bytes or zero if EOF, otherwise -1
 *
 * @security-level APPLICATION
 */
int32_t Read(File_t fd, void* buffer, uint64_t length);
int32_t ReadS(File_t fd, void* buffer, uint64_t length, FsReadStatus* outStatus);

/**
 * Writes bytes from the buffer to the file.
 *
 * @param fd:               the file descriptor
 * @param buffer:           the source buffer
 * @param length:           the length in bytes
 * @param-opt outStatus:    filled with one of the {FsWriteStatus} codes
 * @return if successful the number of bytes that were written, otherwise -1
 *
 * @security-level APPLICATION
 */
int32_t Write(File_t fd, const void* buffer, uint64_t length);
int32_t WriteS(File_t fd, const void* buffer, uint64_t length, FsWriteStatus* outStatus);

/**
 * Returns the next transaction id that can be used for messaging.
 * When sending a message, a transaction can be added so that one can wait
 * for an answer with the same transaction. This method always returns a
 * new transaction id each time it is called and is thread-safe.
 *
 * @return a new transaction id
 *
 * @security-level APPLICATION
 */
MessageTransaction GetMessageTxId();

/**
 * Allocates a memory region with the size of at least the given
 * size in bytes. This region can for example be used as shared memory.
 *
 * Allocating memory using this call makes the requesting process the physical owner of the
 * pages in its virtual space (important for unmapping).
 *
 * @param size:     the size in bytes
 * @return a pointer to the allocated memory region, or nullptr if failed
 *
 * @security-level APPLICATION
 */
void* AllocMem(int32_t size);

/**
 * Shares a memory area with another process.
 *
 * @param memory:   a pointer to the memory area to share
 * @param size:     the size of the memory area
 * @param pid:      the id of the target process
 * @return a pointer to the shared memory location within the target address space
 *
 * @security-level APPLICATION
 */
void* ShareMem(void* memory, int32_t size, Pid pid);

/**
 * Yields, causing a switch to the next process.
 *
 * @security-level APPLICATION
 */
void Yield();

/**
 * Sleeps for the given amount of milliseconds.
 *
 * @param ms:       the milliseconds to sleep
 *
 * @security-level APPLICATION
 */
void Sleep(uint64_t ms);

/**
 * Retrieves the current process id.
 *
 * @return the id of the executing process
 *
 * @security-level APPLICATION
 */
uint32_t GetPid();

/**
 * Retrieves the current thread id. If this thread is the main
 * thread of the process, the id is the same as the process id.
 *
 * @return the id of the executing thread
 *
 * @security-level APPLICATION
 */
uint32_t GetTid();

/**
 * Retrieves the thread name from thread id number
 *
 * @param threadID:     id of thread that we can get the identifier
 * @param out:          buffer where call store the string
 *
 * @security-level APPLICATION
 */
void TaskGetIdentifier(Tid threadID, char* out);

/**
 * Retrieves the process id for a thread id.
 *
 * @param tid the thread id
 * @return the process id of the thread, or -1
 *
 * @security-level APPLICATION
 */
uint32_t GetPidForTid(uint32_t tid);

/**
 * Quits the process with the given status code.
 *
 * @param code:       the status code
 *
 * @security-level APPLICATION
 */
void Exit(int code);

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @param-opt data:         the parameters for the thread function
 * @param-opt name:         the name for the thread, the name is appended to the default name
 * created by the kernel
 * @param-opt outStatus:    the filled out status
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThread(void* function);
Tid CreateThreadD(void* function, void* data);
Tid CreateThreadN(void* function, const char* name);
Tid CreateThreadDN(void* function, void* data, const char* name);
Tid CreateThreadDS(void* function, void* data, CreateThreadStatus* outStatus);
Tid CreateThreadDNS(void* function, void* data, const char* name, CreateThreadStatus* outStatus);

/**
 * Sends a message to the given task. This means that <len> bytes from the
 * buffer <buf> are copied to a message that is then sent to the <target> task.
 * The message may be no longer than {MESSAGE_MAXIMUM_LENGTH}.
 *
 * The mode specifies how the function shall block:
 * - {MESSAGE_SEND_MODE_BLOCKING} the executing task will bock if the target tasks message queue is
 * full
 * - {MESSAGE_SEND_MODE_NON_BLOCKING} the function will return {MESSAGE_SEND_STATUS_QUEUE_FULL} if
 * the target tasks message queue is full
 *
 * @param target:       id of the target task
 * @param buffer:      message content buffer
 * @param length:      number of bytes to copy from the buffer
 * @param-opt mode:    determines how the function blocks when given, default is
 * {MESSAGE_SEND_MODE_BLOCKING}
 * @param-opt tx:      transaction id
 * @return one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
MessageSendStatus SendMessage(Tid target, void* buffer, size_t length);
MessageSendStatus SendMessageM(Tid target, void* buffer, size_t length, MessageSendMode mode);
MessageSendStatus SendMessageT(Tid target, void* buffer, size_t length, MessageTransaction tx);
MessageSendStatus
SendMessageTM(Tid target, void* buffer, size_t length, MessageTransaction tx, MessageSendMode mode);

/**
 * Receives a message. At maximum <max> bytes will be attempted to be copied to
 * the buffer <buf>. Note that when receiving a message, a buffer with a size of
 * at least the size of {MessageHeader} plus the size of the sent message
 * must be used.
 *
 * After successful completion, the buffer will contain the message header followed by the content
 * of the message.
 * - to access the header, use the buffer pointer: ((MessageHeader * buffer)
 * - to access the content, use the helper macro:  MESSAGE_CONTENT(buffer)
 *
 * The mode specifies how the function shall block:
 * - {MESSAGE_RECEIVE_MODE_BLOCKING} the executing task will block if no messages are available
 * - {MESSAGE_RECEIVE_MODE_NON_BLOCKING} the function will return
 * {MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY} if the executing tasks message queue is empty
 *
 * When a transaction ID is given, only messages that were sent with the same
 * transaction ID will be received.
 *
 * @param buffer:               output buffer
 * @param max:                  maximum number of bytes to copy to the buffer
 * @param-opt mode:             determines how the function blocks when given, default is
 * {MESSAGE_RECEIVE_MODE_BLOCKING}
 * @param-opt tx:               transaction id
 * @param-opt breakCondition:   can be used to break the waiting process by setting its value to 1
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus ReceiveMessage(void* buffer, size_t max);
MessageReceiveStatus ReceiveMessageM(void* buffer, size_t max, MessageReceiveMode mode);
MessageReceiveStatus ReceiveMessageT(void* buffer, size_t max, MessageTransaction tx);
MessageReceiveStatus
ReceiveMessageTM(void* buffer, size_t max, MessageTransaction tx, MessageReceiveMode mode);
MessageReceiveStatus ReceiveMessageTMB(void*              buffer,
                                       size_t             max,
                                       MessageTransaction tx,
                                       MessageReceiveMode mode,
                                       uint8_t*           breakCondition);

/**
 * Registers the executing task for the given identifier.
 *
 * @param identifier:       the name to set
 * @param-opt selected:		dilled with the name selected by the kernel, empty if the provided
 * is accepted
 * @return if it was successful true, if the identifier is taken false
 *
 * @security-level APPLICATION
 */
uint8_t TaskRegisterID(const char* identifier);
uint8_t TaskRegisterIDS(const char* identifier, char* selected);

/**
 * Returns the id of the task that is register for the given identifier.
 *
 * @param identifier:   the identifier to search
 * @return the id of the task, or -1 if no task has this identifier
 *
 * @security-level APPLICATION
 */
Tid TaskGetID(const char* identifier);

/**
 * The executing thread waits for the interrupt request with the given number.
 *
 * @param irq:      the irq number
 *
 * @security-level DRIVER
 */
void WaitForIrq(uint8_t irq);

/**
 * Maps the given physical address to the executing processes address space so
 * it can access it directly.
 *
 * @param address:     the physical memory address that should be mapped
 * @param size:        the size that should be mapped
 * @return a pointer to the mapped area within the executing processes address space
 *
 * @security-level DRIVER
 */
void* MapMmio(void* address, uint32_t size);

/**
 * Unmaps the given memory area.
 *
 * @param area:     a pointer to the area
 *
 * @security-level DRIVER
 */
void Unmap(void* area);

/**
 * Adjusts the program heap break.
 *
 * @param amount:       the value to adjust the break by
 * @param outBrk:       is filled with the result
 * @return whether adjusting was successful
 *
 * @security-level APPLICATION
 */
uint8_t Sbrk(int amount, void** outBrk);

/**
 * Performs a Virtual 8086 BIOS interrupt call.
 *
 * @param interrupt:    number of the interrupt to fire
 * @param in:           input register values
 * @param out:          output register values
 * @return one of the {VM86_CALL_STATUS_*} status codes
 *
 * @security-level DRIVER
 */
Vm86CallStatus CallVm86(uint32_t interrupt, Vm86Registers* in, Vm86Registers* out);

/**
 * Frees a memory area allocated with {LowerMalloc}.
 *
 * @param memory:       a pointer to the memory area
 *
 * @security-level DRIVER
 */
void LowerFree(void* memory);

/**
 * Allocates a memory area in the lower 1MiB. This can be used
 * for example for the virtual 8086 mode.
 *
 * @param size:     the size to allocate
 *
 * @security-level DRIVER
 */
void* LowerMalloc(uint32_t size);

/**
 * Enables or disables logging to the video output.
 *
 * @param enabled:      whether to enable/disable video log
 *
 * @security-level APPLICATION
 */
void SetVideoLog(uint8_t enabled);

/**
 * Spawns a binary from the ramdisk.
 *
 * @param path:             ramdisk-relative path to the binary
 * @param securityLevel:    the security level to set for the spawned process
 * @return one the {RamdiskSpawnStatus} codes
 *
 * @param security-level KERNEL
 */
RamdiskSpawnStatus RamdiskSpawn(const char* path, SecurityLevel securityLevel);

/**
 * Finds a ramdisk entry by its absolute path.
 *
 * @param path:     absolute path
 * @return if found the ramdisk node id, otherwise -1
 *
 * @security-level APPLICATION
 */
int RamdiskFind(const char* path);

/**
 * Finds a ramdisk entry's child by its relative path.
 *
 * @param nodeID:       the node id of the parent
 * @param childName:    the name of the child to find
 * @return if found the ramdisk node id, otherwise -1
 *
 * @security-level APPLICATION
 */
int RamdiskFindChild(int nodeID, const char* childName);

/**
 * Retrieves ramdisk entry info for a specific node.
 *
 * @param nodeID:   ramdisk node id
 * @param out:      is filled with the info
 *
 * @security-level APPLICATION
 */
void RamdiskInfo(int nodeID, RamdiskEntryInfo* out);

/**
 * Reads bytes from a file on the ramdisk.
 *
 * @param nodeID:       ramdisk node id
 * @param offset:       offset within the file
 * @param buffer:       target buffer
 * @param length:       number of bytes to read
 * @return if successful the number of bytes read, otherwise -1
 *
 * @security-level APPLICATION
 */
int RamdiskRead(int nodeID, uint32_t offset, char* buffer, uint32_t length);

/**
 * Returns the number of children of a ramdisk entry.
 *
 * @param nodeID:   ramdisk node id
 * @return the number of children
 *
 * @security-level APPLICATION
 */
int RamdiskChildCount(int nodeID);

/**
 * Returns the id of a child of the given node at the given index.
 *
 * @param nodeID:       ramdisk node id
 * @param index:        child index
 * @return if the child exists the child node id, otherwise -1
 *
 * @security-level APPLICATION
 */
int RamdiskChildAt(int nodeID, int index);

/**
 * Returns the number of milliseconds that scheduler where the current task is executed
 * running
 *
 * @return the number of milliseconds
 *
 * @security-level APPLICATION
 */
uint64_t Millis();

/**
 * Test-call for kernel debugging.
 *
 * @security-level VARIOUS
 */
uint32_t Test(uint32_t test);

/**
 * Forks the current process (only works from the main thread)
 *
 * @return within the executing process the forked processes id is returned,
 * 		within the forked process 0 is returned
 *
 * @security-level APPLICATION
 */
Tid Fork();

/**
 * Joins the task with the given id, making the executing task
 * wait until this task has died.
 *
 * @param tid:      id of the task to join
 *
 * @security-level APPLICATION
 */
void Join(uint32_t tid);

/**
 * Clones a file descriptor in a process to another processes. Creates a new file
 * descriptor in the target process.
 *
 * @param sourceFd:         source file descriptor
 * @param sourcePid:        id of the source process
 * @param targetPid:        id of the target process
 * @param-opt outStatus:    is filled with the status code
 * @return the resulting file descriptor
 *
 * @security-level APPLICATION
 */
File_t CloneFd(File_t sourceFd, Pid sourcePid, Pid targetPid);
File_t CloneFdS(File_t sourceFd, Pid sourcePid, Pid targetPid, FsClonefdStatus* outStatus);

/**
 * Clones a file descriptor in a process to another processes file descriptor value.
 *
 * @param sourceFd:         source file descriptor
 * @param sourcePid:        id of the source process
 * @param targetFd:         target file descriptor
 * @param targetPid:        id of the target process
 * @param-opt outStatus:    is filled with the status code
 * @return the target file descriptor
 *
 * @security-level APPLICATION
 */
File_t CloneFdT(File_t sourceFd, Pid sourcePid, File_t targetFd, Pid targetPid);
File_t CloneFdTS(File_t           sourceFd,
                 Pid              sourcePid,
                 File_t           targetFd,
                 Pid              targetPid,
                 FsClonefdStatus* outStatus);

/**
 * Opens a pipe.
 *
 * @param outWrite:     is filled with the pipes write end
 * @param outRead:      is filled with the pipes read end
 * @param-opt outStatus:    is filled with the status code
 *
 * @security-level APPLICATION
 */
void Pipe(File_t* outWrite, File_t* outRead);
void PipeS(File_t* outWrite, File_t* outRead, FsPipeStatus* outStatus);

/**
 * Stores command line arguments for a created process.
 *
 * @param process:      the process creation identifier
 * @param arguments:    arguments to store
 *
 * @security-level KERNEL
 */
void CliArgsStore(ProcessCreationIdentifier process, char* arguments);

/**
 * Returns and releases the command line arguments for the executing process.
 * This buffer must have a length of at least {PROCESS_COMMAND_LINE_ARGUMENTS_BUFFER_LENGTH} bytes.
 * If no arguments were supplied for the executing process, the buffer is null-terminated only.
 *
 * @param buffer:       target buffer to store the arguments to
 *
 * @security-level KERNEL
 */
void CliArgsRelease(char* buffer);

/**
 * Creates an empty process.
 *
 * @param securityLevel:    the security level to use
 * @return the process creation identifier for the created process
 *
 * @security-level KERNEL
 */
ProcessCreationIdentifier CreateEmptyProcess(SecurityLevel securityLevel);

/**
 * Configures the created empty process during spawning.
 *
 * @param process:          process creation identifier for the target process
 * @param configuration:    the configuration structure
 *
 * @security-level KERNEL
 */
void ConfigureProcess(ProcessCreationIdentifier process, ProcessConfiguration configuration);

/**
 * Creates pages in a process during process creation and maps the created pages in the
 * executing processes address space.
 *
 * @param process:          the process creation identifier
 * @param virtualAddress:   the address to map to
 * @param pages:            number of pages to map
 * @return the virtual address of the pages mapped to the executing processes address space
 *
 * @security-level KERNEL
 */
void* CreatePagesInSpaces(ProcessCreationIdentifier process, uint32_t virtualAddress, int pages);

/**
 * Creates a thread-local-storage area for a process and copies/zeroes the given amount of bytes
 * from the content.
 *
 * @param process:      the process creation identifier
 * @param content:      master copy content
 * @param copysize:     number of bytes to copy from content
 * @param zerosize:     total size (including bytes to zero)
 * @param alignment:    tls alignment
 * @return whether writing was successful
 *
 * @security-level KERNEL
 */
uint8_t WriteTlsMasterForProcess(ProcessCreationIdentifier process,
                                 uint8_t*                  content,
                                 uint32_t                  copysize,
                                 uint32_t                  totalsize,
                                 uint32_t                  alignment);

/**
 * Attaches and therefore starts a process during process creation at the given execution point.
 *
 * @param process:      the process creation identifier
 * @param eip:          execution entry point
 *
 * @security-level KERNEL
 */
void AttachCreatedProcess(ProcessCreationIdentifier process, uint32_t eip);

/**
 * Cancels process creation.
 *
 * @param process:      the process creation identifier
 *
 * @security-level KERNEL
 */
void CancelProcessCreation(ProcessCreationIdentifier process);

/**
 * Retrieves the pid for a created process. Only usable if the process was attached.
 *
 * @param process:      the process creation identifier
 * @return the pid of the created process.
 *
 * @security-level KERNEL
 */
Pid GetCreatedProcessID(ProcessCreationIdentifier process);

/**
 * Creates a mountpoint and registers the current thread as its file system delegate.
 *
 * @param name:                     the wanted name
 * @param physMountpointID:         the physical id to set for the mountpoint
 * @param outMountpointID:          is filled with the node id of the mountpoint on success
 * @param outTransactionStorage:    is filled with the address of the transaction storage
 * @return one of the {FsRegisterAsDelegateStatus} codes
 *
 * @security-level DRIVER
 */
FsRegisterAsDelegateStatus FsRegisterAsDelegate(const char* name,
                                                FsPhysID    physMountpointID,
                                                FsVirtID*   outMountpointID,
                                                Address*    outTransactionStorage);

/**
 * Updates the status for a filesystem transaction.
 *
 * @param id:       the transaction id
 * @param status:   the transaction status
 *
 * @security-level DRIVER
 */
void FsSetTransactionStatus(FsTransactionID id, FsTransactionStatus status);

/**
 * Creates a virtual filesystem node.
 *
 * @param parent:           id of the parent node
 * @param name:             the node name
 * @param type:             one of the FsNodeType types
 * @param fsID:             the filesystem id of the node
 * @param outCreatedID:     id of the created node
 * @return one of the {FsCreateNodeStatus} codes
 *
 * @security-level DRIVER
 */
FsCreateNodeStatus
FsCreateNode(uint32_t parent, char* name, FsNodeType type, uint64_t fsID, uint32_t* outCreatedID);

/**
 * Registers the <handler> routine as the handler for the <irq>.
 *
 * @param irq:          IRQ number to handle
 * @param handler:      handler routine to call
 * @return one of the {RegisterIrqHandlerStatus} codes
 *
 * @security-level DRIVER
 */
RegisterIrqHandlerStatus RegisterIrqHandler(uint8_t irq, void (*handler)(uint8_t));

/**
 * Restores the interruption state (for example after signal/irq handling) of the current thread.
 */
void RestoreInterruptedState();

/**
 * Registers the <handler> routine as the handler for the <irq>.
 *
 * @param signal:       signal to handle
 * @param handler:      handler routine to call
 * @return:             previously registered handler
 *
 * @security-level DRIVER
 */
void* RegisterSignalHandler(int signal, void* handler);

/**
 * Raises the <signal> in the <process>.
 *
 * @param process:  target process
 * @param signal:   signal number
 * @return one of the {RaiseSignalStatus} codes
 *
 * @security-level APPLICATION
 */
RaiseSignalStatus RaiseSignal(Pid process, int signal);

/**
 * Performs an ename call to the kernel
 * to get version and version of kernel and loader
 *
 * @param ename:    a {EvaName} buffer where call store infos
 *
 * @security-level APPLICATION
 */
void Ename(EvaName* ename);

/**
 * Performs Sysinfo call to the kernel to get basic
 * information about the hardware of the system
 *
 * @param info:    a {EvaSysInfo} buffer where call store infos
 *
 * @security-level APPLICATION
 */
void Sysinfo(EvaSysInfo* info);

/**
 * Call the kernel to ask the count of the thread in the system
 *
 * @param-opt type:		the type of thread that we want the count
 * @return the thread count
 */
uint32_t GetThreadCount();
uint32_t GetThreadCountT(ThreadType type);

/**
 * Call the kernel to store on <buffer>,
 * of size <length> the tids of the threads
 *
 * @param buffer:		an userspace buffer of integers
 * @param length:		the size of the buffer, use the <GetThreadCount> call to know the
 * size
 * @param-opt type:		the type of thread to get (default all threads)
 * @return the index of the last copied element
 */
uint32_t GetThreadTids(Tid* buffer, uint32_t length);
uint32_t GetThreadTidsT(Tid* buffer, uint32_t length, ThreadType type);

/**
 * Call the kernel to get the descriptor of a specific thread
 *
 * @param id:			the id of the thread that want to get the descriptor
 * @param descriptor:	pointer to a valid {ThreadDescriptor} descriptor
 * @return whether the operation success
 */
uint8_t GetThreadDescriptor(Tid id, ThreadDescriptor* descriptor);

/**
 * Call the kernel to ask the count of the processes in the system
 *
 * @return the process count
 */
uint32_t GetProcessCount();

/**
 * Call the kernel to store on <buffer>,
 * of size <length> the tids of the threads
 *
 * @param buffer:	an userspace buffer of integers
 * @param length:	the size of the buffer, use the <GetThreadCount> call to know the size
 * @return the index of the last copied element
 */
uint32_t GetProcessPids(Pid* buffer, uint32_t length);

/**
 * Call the kernel to get the descriptor of a specific thread
 *
 * @param id:			the id of the thread that want to get the descriptor
 * @param descriptor:	pointer to a valid {ProcessDescriptor} descriptor
 * @return whether the operation success
 */
uint8_t GetProcessDescriptor(Pid id, ProcessDescriptor* descriptor);

/**
 * Call the kernel to get the count of pci device
 *
 * @return the pci devices count
 */
uint32_t GetPciDeviceCount();

/**
 * Call the kernel to get the PCI device header by position
 *
 * @param position:		the position of the device
 * @param header:		a pointer to a valid {PciDeviceHeader} header
 * @return whether the operation success
 */
uint8_t GetPciDevice(uint32_t position, PciDeviceHeader* header);

__END_C

#endif
