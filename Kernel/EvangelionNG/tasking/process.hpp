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

#ifndef EVA_MULTITASKING_PROCESS_
#define EVA_MULTITASKING_PROCESS_

#include "eva/types.h"

#include <memory/collections/AddressRangePool.hpp>
#include <system/smp/GlobalLock.hpp>
#include <tasking/thread.hpp>
#include <utils/ListEntry.hpp>

/**
 * Constants used as flags on virtual ranges of processes
 */
#define PROC_VIRTUAL_RANGE_FLAG_NONE           0
#define PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER 1

/**
 * signal handler descriptor
 * a signal handler is a userspace registered method
 * that manage a specific signal sended by another process
 */
struct SignalHandler {
public:
    uintptr_t handler  = 0;
    uintptr_t callback = 0;
    Tid       threadID = 0;
};

/**
 * Process descriptor.
 * The Process in Ghost/Evangelion arch is the container
 * of the program data, from it's image to it's execution data.
 * Processes are not schedulated, only threads, infact each process have
 * at least one thread called main, that executes the main flow of the loaded program
 */
class Process {
public:
    /**
     * creates a new empty process descriptor
     *
     * @param securityLevel:	the security level of the new process
     */
    Process(SecurityLevel securityLevel);

    /**
     * process destructor
     */
    ~Process();

    /**
     * process that execute this
     */
    Process* parent;

    /**
     * main thread of the process,
     * this executes the main of the program
     */
    Thread* main;

    /**
     * memory allocation informations
     */
    AddressRangePool virtualRanges; // process memory ranges
    VirtualAddress   imageStart;    // the virtual address of the image start
    VirtualAddress   imageEnd;      // the virtual address of the image end
    VirtualAddress   heapStart;     // begin of the process heap
    VirtualAddress   heapBreak;     // end of the process heap
    uint32_t         heapPages;     // heap pages count

    /**
     * thread-local storage copy
     */
    VirtualAddress tlsMasterInProcLocation; // master copy of the tls
    VirtualAddress tlsMasterCopysize;       // tls copy size
    VirtualAddress tlsMasterTotalsize;      // local-storage total size
    VirtualAddress tlsMasterAlignment;      // tls alignment

    /**
     * current process page directory
     */
    PageDirectory pageDirectory; // current space

    /**
     * human readable strings informations
     */
    char* cliArguments;     // command line arguments, used in process creation
    char* workingDirectory; // current working directory
    char* sourcePath;       // source path of the process

    /**
     * process security level
     */
    SecurityLevel securityLevel; // security level ode of the process
    uint8_t       isServer;      // boolean flag to understand if the process is a kernel server

    /**
     * signal handlers of the process, called irq or signal interruption
     */
    SignalHandler signalHandlers[SIG_COUNT];

    /**
     * copy the provided path to the internal source path property
     *
     * @param path:		the path of the process
     */
    void setPath(const char* path);
};

#endif
