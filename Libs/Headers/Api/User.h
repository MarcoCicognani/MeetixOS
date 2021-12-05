/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api/Common.h>
#include <Api/FileSystem.h>
#include <Api/Info.h>
#include <Api/IPC.h>
#include <Api/Kernel.h>
#include <Api/Ramdisk.h>
#include <Api/Syscalls/CallsData.h>
#include <Api/System.h>
#include <Api/Types.h>
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
void s_atomic_lock(bool* atom);
void s_atomic_lock_dual(bool* atom1, bool* atom2);

/**
 * Trys to perform atomic wait. If the lock is already locked, the function
 * returns 0. Otherwise, the lock is set as in {s_atomic_lock} and the
 * function returns 1.
 *
 * @param atom:         the atom to use
 * @param-opt atom2:    the second atom to use
 *
 * @security-level APPLICATION
 */
bool s_atomic_try_lock(bool* atom);
bool s_atomic_try_lock_dual(bool* atom1, bool* atom2);

/**
 * Performs an atomic block. If the atom is true, the executing task must
 * wait until the task that owns the atom has finished its work and sets
 * it to false. Different from the {s_atomic_lock}, the atom is not changed.
 *
 * @param atom:         the atom to use
 * @param-opt atom2:    the second atom to use
 *
 * @security-level APPLICATION
 */
void s_atomic_block(bool* atom);
void s_atomic_block_dual(bool* atom1, bool* atom2);

/**
 * Spawns a program binary.
 *
 * @param path:             absolute path of the executable
 * @param args:             unparsed arguments
 * @param work_dir:          working directory for the execution
 * @param security_level:    security level to s_spawn the process to
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
s_spawn(const char* path, const char* args, const char* work_dir, SecurityLevel security_level);
SpawnStatus s_spawn_p(const char*   path,
                      const char*   args,
                      const char*   work_dir,
                      SecurityLevel security_level,
                      Pid*          out_pid);
SpawnStatus s_spawn_po(const char*   path,
                       const char*   args,
                       const char*   work_dir,
                       SecurityLevel security_level,
                       Pid*          out_pid,
                       FileHandle    out_stdio[3]);
SpawnStatus s_spawn_poi(const char*   path,
                        const char*   args,
                        const char*   work_dir,
                        SecurityLevel security_level,
                        Pid*          out_pid,
                        FileHandle    out_stdio[3],
                        FileHandle    in_stdio[3]);

/**
 * Register the current process as a kernel
 * server and return one of the {RegisterAsServerStatus} codes.
 * Update the security level of the process and permits to increase it since the
 * {SECURITY_LEVEL_SERVER}
 *
 * @param server_identifier:		the name of the server
 * @param security_level:				the new security level for the process
 * @return one of the {RegisterAsServerStatus} codes
 */
RegisterAsServerStatus s_register_as_server(const char*   server_identifier,
                                            SecurityLevel security_level);

/**
 * Provides a standard interface to communicate with servers,
 * first the call send a message to the server identified by <serverIdentifier>,
 * if it exists, then wait with a receive message the response of the server, that provides data by
 * the <data> parameter
 *
 * @param server_identifier:			the name of the server
 * @param manage_command:					a command to send to the server
 * @param data_ptr:						a pointer the data that process the
 * server called
 * @param data_len:					the length of the data buffer
 * @return one of the {ServerManageStatus} codes.
 */
ServerManageStatus s_server_manage(const char*         server_identifier,
                                   ServerManageCommand manage_command,
                                   void*               data_ptr,
                                   usize               data_len);

/**
 * call spawner to reboot the computer
 *
 * @security-level KERNEL
 */
void s_reboot_system();

/**
 * call spawner to shutdown the computer
 *
 * @security-level KERNEL
 */
void s_shutdown_system();

/**
 * Kills a process.
 *
 * @param proc_id:      the process id
 * @return one of the {KillStatus} codes
 *
 * @security-level APPLICATION
 */
KillStatus s_kill(Pid proc_id);

/**
 * Prints a message to the s_log.
 *
 * @param message:      the message to s_log
 *
 * @security-level APPLICATION
 */
void s_log(const char* message);

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
void do_syscall(usize call, usize data);

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
FileHandle s_open(const char* path);
FileHandle s_open_f(const char* path, i32 flags);
FileHandle s_open_fs(const char* path, i32 flags, FsOpenStatus* out_status);
FileHandle s_open_fms(const char* path, i32 flags, i32 mode, FsOpenStatus* out_status);

/**
 * Closes a file.
 *
 * @param fd:   the file descriptor to close
 * @return one of the {FsCloseStatus} codes
 *
 * @security-level APPLICATION
 */
FsCloseStatus s_close(FileHandle fd);

/**
 * Retrieves the length of a file in bytes.
 *
 * @param fd:                   the file descriptor
 * @param-opt outStatus:        is filled with the status code
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
i64 s_length(FileHandle fd);
i64 s_length_s(FileHandle fd, FsLengthStatus* out_status);

/**
 * Opens a directory.
 *
 * @param path:             path of the directory
 * @param-opt outStatus:    is filled with the out status
 * @return pointer to a directory iterator, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryIterator* s_open_directory(const char* path);
FsDirectoryIterator* s_open_directory_s(const char* path, FsOpenDirectoryStatus* out_status);

/**
 * Reads the next entry of the directory.
 *
 * @param iterator:         the directory iterator
 * @param-opt outStatus:    is filled with the status code
 * @return a directory entry structure, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryEntry* s_read_directory(FsDirectoryIterator* iterator);
FsDirectoryEntry* s_read_directory_s(FsDirectoryIterator*   iterator,
                                     FsReadDirectoryStatus* out_status);

/**
 * Closes a directory.
 *
 * @param iterator:     the directory iterator
 *
 * @security-level APPLICATION
 */
void s_close_directory(FsDirectoryIterator* iterator);

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
i64 s_flength(const char* path);
i64 s_flength_s(const char* path, bool follow_symlinks);
i64 s_flength_ss(const char* path, bool follow_symlinks, FsLengthStatus* out_status);

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
i64 s_seek(FileHandle fd, i64 off, FsSeekMode mode);
i64 s_seek_s(FileHandle fd, i64 off, FsSeekMode mode, FsSeekStatus* out_status);

/**
 * Retrieves the current offset within a file.
 *
 * @param fd:               the file descriptor
 * @param-opt outStatus:    is filled with the status
 * @return if successful returns the current absolute offset, otherwise -1
 *
 * @security-level APPLICATION
 */
i64 s_tell(FileHandle fd);
i64 s_tell_s(FileHandle fd, FsTellStatus* out_status);

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
SetWorkingDirectoryStatus s_set_working_directory(const char* path);
SetWorkingDirectoryStatus s_set_working_directory_p(const char*               path,
                                                    ProcessCreationIdentifier process);

/**
 * Retrieves the working directory for the current process.
 *
 * @param path:         buffer of at least <maxlen> or {PATH_MAX} bytes size
 * @param-opt maxlen:   length of the buffer in bytes
 * @return whether the action was successful
 *
 * @security-level APPLICATION
 */
GetWorkingDirectoryStatus s_get_working_directory(char* buffer);
GetWorkingDirectoryStatus s_get_working_directory_l(char* buffer, usize buffer_len);

/**
 * Retrieves the directory of the executable when available, otherwise an empty
 * string is written to the buffer.
 *
 * @param path:     buffer of at least {PATH_MAX} bytes size
 *
 * @security-level APPLICATION
 */
void s_get_executable_path(char* buffer);

/**
 * Reads bytes from the file to the buffer.
 *
 * @param fd:               the file descriptor
 * @param buffer:           the target buffer
 * @param buffer_len:           the length in bytes
 * @param-opt outStatus:    filled with one of the {FsReadStatus} codes
 * @return if the read was successful the length of bytes or zero if EOF, otherwise -1
 *
 * @security-level APPLICATION
 */
usize s_read(FileHandle fd, void* buffer, usize buffer_len);
usize s_read_s(FileHandle fd, void* buffer, usize buffer_len, FsReadStatus* out_status);

/**
 * Writes bytes from the buffer to the file.
 *
 * @param fd:               the file descriptor
 * @param buffer:           the source buffer
 * @param buffer_len:           the length in bytes
 * @param-opt outStatus:    filled with one of the {FsWriteStatus} codes
 * @return if successful the number of bytes that were written, otherwise -1
 *
 * @security-level APPLICATION
 */
usize s_write(FileHandle fd, const void* buffer, usize buffer_len);
usize s_write_s(FileHandle fd, const void* buffer, usize buffer_len, FsWriteStatus* out_status);

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
MessageTransaction s_get_message_tx_id();

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
void* s_alloc_mem(usize size);

/**
 * Shares a memory area with another process.
 *
 * @param memory:   a pointer to the memory area to share
 * @param size:     the size of the memory area
 * @param proc_id:      the id of the target process
 * @return a pointer to the shared memory location within the target address space
 *
 * @security-level APPLICATION
 */
void* s_share_mem(void* memory, usize size, Pid proc_id);

/**
 * Yields, causing a switch to the next process.
 *
 * @security-level APPLICATION
 */
void s_yield();

/**
 * Sleeps for the given amount of milliseconds.
 *
 * @param ms:       the milliseconds to sleep
 *
 * @security-level APPLICATION
 */
void s_sleep(usize ms);

/**
 * Retrieves the current process id.
 *
 * @return the id of the executing process
 *
 * @security-level APPLICATION
 */
Pid s_get_pid();

/**
 * Retrieves the current thread id. If this thread is the main
 * thread of the process, the id is the same as the process id.
 *
 * @return the id of the executing thread
 *
 * @security-level APPLICATION
 */
Tid s_get_tid();

/**
 * Retrieves the thread name from thread id number
 *
 * @param thread_id:     id of thread that we can get the identifier
 * @param out:          buffer where call store the string
 *
 * @security-level APPLICATION
 */
void s_get_task_identifier(Tid thread_id, char* out);

/**
 * Retrieves the process id for a thread id.
 *
 * @param thread_id the thread id
 * @return the process id of the thread, or -1
 *
 * @security-level APPLICATION
 */
Pid s_get_pid_for_tid(Tid thread_id);

/**
 * Quits the process with the given status code.
 *
 * @param exit_code:       the status code
 *
 * @security-level APPLICATION
 */
void s_exit(int exit_code);

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param func_ptr:         the thread function
 * @param-opt data:         the parameters for the thread function
 * @param-opt name:         the name for the thread, the name is appended to the default name
 * created by the kernel
 * @param-opt outStatus:    the filled out status
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid s_create_thread(void* func_ptr);
Tid s_create_thread_d(void* func_ptr, void* data);
Tid s_create_thread_n(void* func_ptr, const char* name);
Tid s_create_thread_dn(void* func_ptr, void* data, const char* name);
Tid s_create_thread_ds(void* func_ptr, void* data, CreateThreadStatus* out_status);
Tid s_create_thread_dns(void*               func_ptr,
                        void*               data,
                        const char*         name,
                        CreateThreadStatus* out_status);

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
 * @param buffer_len:      number of bytes to copy from the buffer
 * @param-opt mode:    determines how the function blocks when given, default is
 * {MESSAGE_SEND_MODE_BLOCKING}
 * @param-opt tx:      transaction id
 * @return one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
MessageSendStatus s_send_message(Tid target, void* buffer, usize buffer_len);
MessageSendStatus
s_send_message_m(Tid target, void* buffer, usize buffer_len, MessageSendMode send_mode);
MessageSendStatus
s_send_message_t(Tid target, void* buffer, usize buffer_len, MessageTransaction tx);
MessageSendStatus s_send_message_tm(Tid                target,
                                    void*              buffer,
                                    usize              buffer_len,
                                    MessageTransaction tx,
                                    MessageSendMode    send_mode);

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
 * @param buffer_len:                  maximum number of bytes to copy to the buffer
 * @param-opt mode:             determines how the function blocks when given, default is
 * {MESSAGE_RECEIVE_MODE_BLOCKING}
 * @param-opt tx:               transaction id
 * @param-opt breakCondition:   can be used to break the waiting process by setting its value to 1
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus s_receive_message(void* buffer, usize buffer_len);
MessageReceiveStatus
s_receive_message_m(void* buffer, usize buffer_len, MessageReceiveMode receive_mode);
MessageReceiveStatus s_receive_message_t(void* buffer, usize buffer_len, MessageTransaction tx);
MessageReceiveStatus s_receive_message_tm(void*              buffer,
                                          usize              buffer_len,
                                          MessageTransaction tx,
                                          MessageReceiveMode receive_mode);
MessageReceiveStatus s_receive_message_tmb(void*              buffer,
                                           usize              buffer_len,
                                           MessageTransaction tx,
                                           MessageReceiveMode receive_mode,
                                           bool*              break_condition);

/**
 * Registers the executing task for the given identifier.
 *
 * @param new_identifier:       the name to set
 * @param-opt selected:		dilled with the name selected by the kernel, empty if the provided
 * is accepted
 * @return if it was successful true, if the identifier is taken false
 *
 * @security-level APPLICATION
 */
bool s_task_register_id(const char* new_identifier);
bool s_task_register_ids(const char* new_identifier, char* selected);

/**
 * Returns the id of the task that is register for the given identifier.
 *
 * @param identifier:   the identifier to search
 * @return the id of the task, or -1 if no task has this identifier
 *
 * @security-level APPLICATION
 */
Tid s_task_get_id(const char* identifier);

/**
 * The executing thread waits for the interrupt request with the given number.
 *
 * @param irq:      the irq number
 *
 * @security-level DRIVER
 */
void s_wait_for_irq(u8 irq);

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
void* s_map_mmio(void* address, usize size);

/**
 * Unmaps the given memory area.
 *
 * @param area:     a pointer to the area
 *
 * @security-level DRIVER
 */
void s_unmap_mem(void* area);

/**
 * Adjusts the program heap break.
 *
 * @param amount:       the value to adjust the break by
 * @param out_break:       is filled with the result
 * @return whether adjusting was successful
 *
 * @security-level APPLICATION
 */
bool s_set_break(isize amount, void** out_break);

/**
 * Performs a Virtual 8086 BIOS interrupt call.
 *
 * @param interrupt:    number of the interrupt to fire
 * @param in_regs:           input register values
 * @param out_regs:          output register values
 * @return one of the {VM86_CALL_STATUS_*} status codes
 *
 * @security-level DRIVER
 */
Vm86CallStatus s_call_vm_86(usize interrupt, VM86Registers* in_regs, VM86Registers* out_regs);

/**
 * Frees a memory area allocated with {s_lower_malloc}.
 *
 * @param memory:       a pointer to the memory area
 *
 * @security-level DRIVER
 */
void s_lower_free(void* memory);

/**
 * Allocates a memory area in the lower 1MiB. This can be used
 * for example for the virtual 8086 mode.
 *
 * @param size:     the size to allocate
 *
 * @security-level DRIVER
 */
void* s_lower_malloc(usize size);

/**
 * Enables or disables logging to the video output.
 *
 * @param enabled:      whether to enable/disable video s_log
 *
 * @security-level APPLICATION
 */
void s_set_video_log(bool enabled);

/**
 * Returns the number of milliseconds that scheduler where the current task is executed
 * running
 *
 * @return the number of milliseconds
 *
 * @security-level APPLICATION
 */
u64 s_millis();

/**
 * s_test-call for kernel debugging.
 *
 * @security-level VARIOUS
 */
usize s_test(usize test);

/**
 * Forks the current process (only works from the main thread)
 *
 * @return within the executing process the forked processes id is returned,
 * 		within the forked process 0 is returned
 *
 * @security-level APPLICATION
 */
Tid s_fork();

/**
 * Joins the task with the given id, making the executing task
 * wait until this task has died.
 *
 * @param thread_id:      id of the task to join
 *
 * @security-level APPLICATION
 */
void s_join(Tid thread_id);

/**
 * Clones a file descriptor in a process to another processes. Creates a new file
 * descriptor in the target process.
 *
 * @param source_fd:         source file descriptor
 * @param source_proc_id:        id of the source process
 * @param target_proc_id:        id of the target process
 * @param-opt outStatus:    is filled with the status code
 * @return the resulting file descriptor
 *
 * @security-level APPLICATION
 */
FileHandle s_clone_fd(FileHandle source_fd, Pid source_proc_id, Pid target_proc_id);
FileHandle s_clone_fd_s(FileHandle       source_fd,
                        Pid              source_proc_id,
                        Pid              target_proc_id,
                        FsCloneFdStatus* out_status);

/**
 * Clones a file descriptor in a process to another processes file descriptor value.
 *
 * @param source_fd:         source file descriptor
 * @param source_proc_id:        id of the source process
 * @param target_fd:         target file descriptor
 * @param target_proc_id:        id of the target process
 * @param-opt outStatus:    is filled with the status code
 * @return the target file descriptor
 *
 * @security-level APPLICATION
 */
FileHandle
s_clone_fd_t(FileHandle source_fd, Pid source_proc_id, FileHandle target_fd, Pid target_proc_id);
FileHandle s_clone_fd_ts(FileHandle       source_fd,
                         Pid              source_proc_id,
                         FileHandle       target_fd,
                         Pid              target_proc_id,
                         FsCloneFdStatus* out_status);

/**
 * Opens a pipe.
 *
 * @param out_write:     is filled with the pipes write end
 * @param out_read:      is filled with the pipes read end
 * @param-opt outStatus:    is filled with the status code
 *
 * @security-level APPLICATION
 */
void s_pipe(FileHandle* out_write, FileHandle* out_read);
void s_pipe_s(FileHandle* out_write, FileHandle* out_read, FsPipeStatus* out_status);

/**
 * Stores command line arguments for a created process.
 *
 * @param process:      the process creation identifier
 * @param arguments:    arguments to store
 *
 * @security-level KERNEL
 */
void s_cli_args_store(ProcessCreationIdentifier process, char* arguments);

/**
 * Returns and releases the command line arguments for the executing process.
 * This buffer must have a length of at least {PROCESS_COMMAND_LINE_ARGUMENTS_BUFFER_LENGTH} bytes.
 * If no arguments were supplied for the executing process, the buffer is null-terminated only.
 *
 * @param buffer:       target buffer to store the arguments to
 *
 * @security-level KERNEL
 */
void s_cli_args_release(char* buffer);

/**
 * Creates an empty process.
 *
 * @param security_level:    the security level to use
 * @return the process creation identifier for the created process
 *
 * @security-level KERNEL
 */
ProcessCreationIdentifier s_create_empty_process(SecurityLevel security_level);

/**
 * Configures the created empty process during spawning.
 *
 * @param process:          process creation identifier for the target process
 * @param configuration:    the configuration structure
 *
 * @security-level KERNEL
 */
void s_configure_process(ProcessCreationIdentifier process, ProcessConfiguration configuration);

/**
 * Creates pages in a process during process creation and maps the created pages in the
 * executing processes address space.
 *
 * @param process:          the process creation identifier
 * @param virtual_address:   the address to map to
 * @param pages_count:            number of pages to map
 * @return the virtual address of the pages mapped to the executing processes address space
 *
 * @security-level KERNEL
 */
void* s_create_pages_in_spaces(ProcessCreationIdentifier process,
                               Address                   virtual_address,
                               usize                     pages_count);

/**
 * Creates a thread-local-storage area for a process and copies/zeroes the given amount of bytes
 * from the content.
 *
 * @param process:      the process creation identifier
 * @param content:      master copy content
 * @param copy_size:     number of bytes to copy from content
 * @param zerosize:     total size (including bytes to zero)
 * @param alignment:    tls alignment
 * @return whether writing was successful
 *
 * @security-level KERNEL
 */
bool s_write_tls_master_for_process(ProcessCreationIdentifier process,
                                    u8*                       content,
                                    usize                     copy_size,
                                    usize                     total_size,
                                    usize                     alignment);

/**
 * Attaches and therefore starts a process during process creation at the given execution point.
 *
 * @param process:      the process creation identifier
 * @param eip:          execution entry point
 *
 * @security-level KERNEL
 */
void s_attach_created_process(ProcessCreationIdentifier process, Address eip);

/**
 * Cancels process creation.
 *
 * @param process:      the process creation identifier
 *
 * @security-level KERNEL
 */
void s_cancel_process_creation(ProcessCreationIdentifier process);

/**
 * Retrieves the pid for a created process. Only usable if the process was attached.
 *
 * @param process:      the process creation identifier
 * @return the pid of the created process.
 *
 * @security-level KERNEL
 */
Pid s_get_created_process_id(ProcessCreationIdentifier process);

/**
 * Creates a mountpoint and registers the current thread as its file system delegate.
 *
 * @param name:                     the wanted name
 * @param phys_mountpoint_id:         the physical id to set for the mountpoint
 * @param out_mountpoint_id:          is filled with the node id of the mountpoint on success
 * @param out_transaction_storage:    is filled with the address of the transaction storage
 * @return one of the {FsRegisterAsDelegateStatus} codes
 *
 * @security-level DRIVER
 */
FsRegisterAsDelegateStatus s_fs_register_as_delegate(const char* name,
                                                     FsPhysID    phys_mountpoint_id,
                                                     FsVirtID*   out_mountpoint_id,
                                                     Address*    out_transaction_storage);

/**
 * Updates the status for a filesystem transaction.
 *
 * @param id:       the transaction id
 * @param status:   the transaction status
 *
 * @security-level DRIVER
 */
void s_fs_set_transaction_status(FsTransactionID id, FsTransactionStatus status);

/**
 * Creates a virtual filesystem node.
 *
 * @param parent:           id of the parent node
 * @param name:             the node name
 * @param type:             one of the FsNodeType types
 * @param fs_id:             the filesystem id of the node
 * @param out_created_id:     id of the created node
 * @return one of the {FsCreateNodeStatus} codes
 *
 * @security-level DRIVER
 */
FsCreateNodeStatus s_fs_create_node(usize       parent,
                                    const char* name,
                                    FsNodeType  type,
                                    usize       fs_id,
                                    usize*      out_created_id);

/**
 * Registers the <handler> routine as the handler for the <irq>.
 *
 * @param irq:          IRQ number to handle
 * @param handler:      handler routine to call
 * @return one of the {RegisterIrqHandlerStatus} codes
 *
 * @security-level DRIVER
 */
RegisterIrqHandlerStatus s_register_irq_handler(u8 irq, void (*handler)(u8));

/**
 * Restores the interruption state (for example after signal/irq handling) of the current thread.
 */
void s_restore_interrupted_state();

/**
 * Registers the <handler> routine as the handler for the <irq>.
 *
 * @param signal:       signal to handle
 * @param handler:      handler routine to call
 * @return:             previously registered handler
 *
 * @security-level DRIVER
 */
void* s_register_signal_handler(int signal, void* handler);

/**
 * Raises the <signal> in the <process>.
 *
 * @param proc_id:  target process
 * @param signal:   signal number
 * @return one of the {RaiseSignalStatus} codes
 *
 * @security-level APPLICATION
 */
RaiseSignalStatus s_raise_signal(Pid proc_id, int signal);

/**
 * Performs an ename call to the kernel
 * to get version and version of kernel and loader
 *
 * @param kernel_name:    a {EvaName} buffer where call store infos
 *
 * @security-level APPLICATION
 */
void s_kernel_name(KernelName* kernel_name);

/**
 * Performs s_system_info call to the kernel to get basic
 * information about the hardware of the system
 *
 * @param system_info:    a {EvaSysInfo} buffer where call store infos
 *
 * @security-level APPLICATION
 */
void s_system_info(SystemInfo* system_info);

/**
 * Call the kernel to ask the count of the thread in the system
 *
 * @param-opt type:		the type of thread that we want the count
 * @return the thread count
 */
usize s_get_thread_count();
usize s_get_thread_count_t(ThreadType thread_type);

/**
 * Call the kernel to store on <buffer>,
 * of size <length> the tids of the threads
 *
 * @param buffer:		an userspace buffer of integers
 * @param buffer_len:		the size of the buffer, use the <s_get_thread_count> call to know
 * the size
 * @param-opt type:		the type of thread to get (default all threads)
 * @return the index of the last copied element
 */
usize s_get_thread_ids(Tid* buffer, usize buffer_len);
usize s_get_thread_ids_t(Tid* buffer, usize buffer_len, ThreadType thread_type);

/**
 * Call the kernel to get the descriptor of a specific thread
 *
 * @param thread_id:			the id of the thread that want to get the descriptor
 * @param descriptor:	pointer to a valid {ThreadDescriptor} descriptor
 * @return whether the operation success
 */
bool s_get_thread_descriptor(Tid thread_id, ThreadDescriptor* descriptor);

/**
 * Call the kernel to ask the count of the processes in the system
 *
 * @return the process count
 */
usize s_get_process_count();

/**
 * Call the kernel to store on <buffer>,
 * of size <length> the tids of the threads
 *
 * @param buffer:	an userspace buffer of integers
 * @param buffer_len:	the size of the buffer, use the <s_get_thread_count> call to know the size
 * @return the index of the last copied element
 */
usize s_get_process_ids(Pid* buffer, usize buffer_len);

/**
 * Call the kernel to get the descriptor of a specific thread
 *
 * @param proc_id:			the id of the thread that want to get the descriptor
 * @param descriptor:	pointer to a valid {ProcessDescriptor} descriptor
 * @return whether the operation success
 */
bool s_get_process_descriptor(Pid proc_id, ProcessDescriptor* descriptor);

/**
 * Call the kernel to get the count of pci device
 *
 * @return the pci devices count
 */
usize s_get_pci_device_count();

/**
 * Call the kernel to get the PCI device header by position
 *
 * @param position:		the position of the device
 * @param header:		a pointer to a valid {PciDeviceHeader} header
 * @return whether the operation success
 */
bool s_get_pci_device(usize position, PCIDeviceHeader* header);

/**
 * @brief Fills the given <DateTime> with the current date-time
 */
bool s_get_date_time(DateTime* date_time);

__END_C
