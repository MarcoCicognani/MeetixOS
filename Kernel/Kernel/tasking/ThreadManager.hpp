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

#ifndef EVA_KERNEL_TASKIN_THREADMANAGER
#define EVA_KERNEL_TASKIN_THREADMANAGER

#include "eva/kernel.h"
#include "eva/stdint.h"

#include <system/ProcessorState.hpp>
#include <tasking/thread.hpp>

/**
 * static class to manage threads
 */
class ThreadManager {
private:
    /**
     * create a thread from process with provided type and return it's instance
     *
     * @param process:		the owner process of the thread
     * @param type:			the thread type (main/vm86/sub)
     * @return the instance of the thread
     */
    static Thread* createThread(Process* process, ThreadType type);

    /**
     * Allocates and initializes a new page directory used for process creation.
     *
     * @return the page directory forked
     */
    static PageDirectory initializePageDirectoryForProcess();

    /**
     * fork the page directory for the created fork thread
     *
     * @param process:				the owner process
     * @param current:				the forked thread
     * @param outKernelStackVirt:	the forked kernel stack
     * @param outUserStackVirt:		the forked user stack
     * @return the physicalAddress of created directory
     */
    static PhysicalAddress forkCurrentPageDirectory(Process*        process,
                                                    Thread*         current,
                                                    VirtualAddress* outKernelStackVirt,
                                                    VirtualAddress* outUserStackVirt);

    /**
     * Allocates the kernel thread stack for a new thread in the given process.
     *
     * @param process:				the owner process of stack
     * @param outUserStackVirt:		the created user stack
     * @return if the operation end fine
     */
    static bool createThreadKernelStack(Process* process, VirtualAddress* outKernelStackVirt);

    /**
     * Allocates the user thread stack for a new thread in the given process.
     *
     * @param process:				the owner process of stack
     * @param outUserStackVirt:		the created user stack
     * @return if the operation end fine
     */
    static bool createThreadUserStack(Process* process, VirtualAddress* outUserStackVirt);

    /**
     * Frees and unmap the memories between the provided addresses
     *
     * @param start:		the start address
     * @param end:			the end address
     * @param ranges:		the pointer to the address range pool of the process, if 0 is provided isn't
     * used
     */
    static void freeAndUnmap(VirtualAddress start, VirtualAddress end, AddressRangePool* ranges);

    /**
     * show all properties of provided task (used in debug cases)
     *
     * @param task:		the task that wqe have to dump
     */
    static void dumpTask(Thread* task);

public:
    /**
     * create a new process with the provided SecurityLevel and linked with the parent
     *
     * @param securityLevel:	the SecurityLevel of the process
     * @param parent:			the parent process
     * @return the created main thread of the process
     */
    static Thread* createProcess(SecurityLevel securityLevel, Process* parent);

    /**
     * create a virtual 8086 process
     *
     * @param interrupt:	the interrupt code
     * @param in:			the provided Vm86Registers instace
     * @param out:			the returned informations
     * @return the created main thread of the process
     */
    static Thread* createProcessVm86(uint8_t interrupt, Vm86Registers& in, Vm86Registers* out);

    /**
     * create a new subthread of the provided process
     *
     * @param process:		the process that is the owner of the new thread
     * @return the new created thread
     */
    static Thread* createSubThread(Process* process);

    /**
     * apply the SecurityLevel to cpu state
     *
     * @param state:			cpu state instance
     * @param securityLevel:	the security level to apply
     */
    static void applySecurityLevel(ProcessorState* state, SecurityLevel securityLevel);

    /**
     * fork the provided process from it's main thread
     *
     * @param task:		the thread to be forked
     * @return the forked thread
     */
    static Thread* fork(Thread* task);

    /**
     * prepare the local storage for the provided thread
     *
     * @param thread:		the thread that shall stored
     */
    static void prepareThreadLocalStorage(Thread* thread);

    /**
     * setup the initial identifier for the task from type
     *
     * @param task:		the task that be setted
     * @param type:		the thread type (main/vm86/sub)
     */
    static void setupInitialIdentifier(Thread* task, ThreadType type);

    /**
     * deletes a task and cleans up all its memory. This task must not be in the
     * scheduling list anymore.
     *
     *	@param task:		the task to destroy
     */
    static void deleteTask(Thread* task);

    /**
     * returns the used heap amount of the process
     *
     * @param task:		the task that we would get the memory amount
     * @return the heap amount in kb
     */
    static uint32_t getProcessHeapSize(Thread* task);

    /**
     * returns the size of the process image
     *
     * @param task:		the task that we would get the memory amount
     * @return the image size of the process on memory
     */
    static uint32_t getProcessImageSize(Thread* task);

    /**
     * returns the thread stack size in kb
     *
     * @param task:		one the process tasks
     * @return the size of the process on memory kb
     */
    static uint32_t getThreadStackSize(Thread* task);
};

#endif
