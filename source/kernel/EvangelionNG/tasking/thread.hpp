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

#ifndef EVA_MULTITASKING_THREAD
#define EVA_MULTITASKING_THREAD

#include <system/ProcessorState.hpp>

#include "eva/kernel.h"
#include "eva/calls/calls.h"
#include "eva/signal.h"
#include "memory/paging.hpp"
#include "memory/collections/AddressRangePool.hpp"

// forward declarations
class Process;
class Waiter;
class Scheduler;

/**
 * Data used by virtual 8086 processes
 */
struct ThreadInformationVm86
{
public:
	/**
	 * empty constructor
	 */
	ThreadInformationVm86() : cpuIf(false), out(0), interruptRecursionLevel(0) {}

	/**
	 * internal data
	 */
	bool cpuIf;							// cpu if flag
	Vm86Registers *out;  				// out register instance
	uint32_t interruptRecursionLevel; 	// level of interrupt recursive
};

/**
 * interruption codes
 * a thread can be interrupted by one of these motivations
 */
enum class ThreadInterruptionInfoType : uint8_t
{
	NONE,
	IRQ,
	SIGNAL
};

/**
 * interruption descriptor
 */
class ThreadInterruptionInfo
{
public:
	/**
	 * empty constructor
	 */
	ThreadInterruptionInfo() :
		cpuStateAddress(0), waitManager(0), type(ThreadInterruptionInfoType::NONE), handledIrq(-1), handledSignal(-1) {}

	/**
	 * cpu infos
	 */
	ProcessorState cpuState;			// saved context
	ProcessorState *cpuStateAddress;	// saved context pointer
	Waiter *waitManager;				// waiter of the thread

	/**
	 * interruption infos
	 */
	ThreadInterruptionInfoType type;	// type of interruption
	uint8_t handledIrq;					// in case of irq interruption contains the number of the irq
	int handledSignal;					// in case of signal interruption contains the number of the signal
};

/**
 * thread descriptor
 * in Ghost/Evangelion arch the thread is the scheduled stuff
 */
class Thread
{
private:
	/**
	 * internal data
	 */
	ThreadInformationVm86 *vm86Information; // used when a process become a VM86
	char *identifier;						// name of the thread

public:
	/**
	 * creates a new thread with the provided type code
	 *
	 * @param type:		the type of thread (main/vm86/sub)
	 */
	Thread(ThreadType type);

	/**
	 * thread destructor
	 */
	~Thread();

	/**
	 * identity informations
	 */
	Tid id;                  // numeric identifier
	bool alive;              // alive state
	ThreadType type;         // thread type (main/vm86/sub)
	ThreadPriority priority; // thread priority on scheduler
	Process *process;        // owner process
	Scheduler *scheduler;    // executor scheduler
	uint64_t rounds;         // time rounding

	/**
	 * waiter informations
	 */
	Waiter *waitManager;    // instance of wait manager
	uint32_t waitCount;     // number of wait time

	/**
	 * thread execution data
	 */
	void *userData;           // data of thread (function parameters ecc)
	void *threadEntry;        // entry to thread execution instructions
	ProcessorState *cpuState; // cpu regex state

	/**
	 * memory informations
	 */
	VirtualAddress kernelStackPageVirt;   // kernel space memory stack
	VirtualAddress kernelStackEsp0;       // kernel space esp stack
	VirtualAddress userStackAreaStart;    // user space stack
	VirtualAddress userThreadAddr;        // copy of virtual address of thread in user space
	VirtualAddress tlsCopyVirt;           // copy of virtual tls
	uint8_t userStackPages;               // pages used by user space stack

	/**
	 * instance of interruption info
	 */
	ThreadInterruptionInfo *interruptionInfo;

	/**
	 * @return the vm86Information instance
	 */
	ThreadInformationVm86 *getVm86Information();

	/**
	 * @return the thread name
	 */
	const char *getIdentifier();

	/**
	 * set a new name for the task
	 *
	 * @param newIdentifier:	the new provided identifier
	 */
	void setIdentifier(const char *newIdentifier);

	/**
	 * create a new wait manager
	 *
	 * @param waitManager:		new wait manager
	 */
	void wait(Waiter *newWaitManager);

	/**
	 * @return true if the waiting is ended, false otherwise
	 */
	bool checkWaiting();

	/**
	 * destroy the current waiter if exists
	 */
	void unwait();

	/**
	 * raise the provided signal
	 *
	 * @param signal:	signal number to handle
	 */
	void raiseSignal(int signal);

	/**
	 * create an irq handler
	 *
	 * @param address:		the address of the handler function
	 * @param irq:			irq code
	 * @param callback:		callback address
	 */
	void enterIrqHandler(uintptr_t address, uint8_t irq, uintptr_t callback);

	/**
	 * create a signal handler
	 *
	 * @param address:		the address of the handler function
	 * @param signal:		irq signal
	 * @param callback:		callback address
	 */
	void enterSignalHandler(uintptr_t address, int signal, uintptr_t callback);

	/**
	 * create the interruption info object if doesn't exist and store to it the waitManager
	 */
	bool startPrepareInterruption();

	/**
	 * create the interruption waiter
	 *
	 * @param address:		address of the interruption
	 * @param callback:		callback address
	 */
	void finishPrepareInterruption(uintptr_t address, uintptr_t callback);

	/**
	 * store cpu states when called interruption
	 */
	void storeForInterruption();

	/**
	 * restore the cpu state after interruption
	 */
	void restoreInterruptedState();
};

#endif
