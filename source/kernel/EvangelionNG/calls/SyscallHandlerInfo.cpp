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

#include <calls/SyscallHandler.hpp>

#include <memory/physical/PPallocator.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/tasking.hpp>
#include <system/pci/pci.hpp>
#include <system/system.hpp>
#include <utils/string.hpp>
#include <BuildConfig.hpp>
#include <eva/info.h>

/**
 * Returns kernel version
 */
SYSCALL_HANDLER(ename)
{
	EvaName *data = (EvaName*) SYSCALL_DATA(currentThread->cpuState);

	// copy loader name & version
	String::copy(data->loaderName, LOADER_NAME);
	data->LversionMJ = L_VERSION_MAJOR;
	data->LversionMN = L_VERSION_MINOR;
	data->LversionPT = L_VERSION_PATCH;

	// copy kernel name & version
	String::copy(data->kernelName, KERNEL_NAME);
	data->versionMJ = VERSION_MAJOR;
	data->versionMN = VERSION_MINOR;
	data->versionSB = VERSION_SUB;
	data->versionPT = VERSION_PATCH;

	return currentThread;
}

/**
 * Returns system specifications
 */
SYSCALL_HANDLER(sysinfo)
{
	EvaSysInfo *data = (EvaSysInfo*) SYSCALL_DATA(currentThread->cpuState);

	// get ram size in KB
	data->totRam = (PPallocator::getInitialAmount() * PAGE_SIZE / 1024);
	data->freeRam = (PPallocator::getFreePageCount() * PAGE_SIZE / 1024);

	// get number of cpu cores
	data->numberOfCore = System::getNumberOfProcessors();

	// get the cpu vendor name
	char vendor[13];
	Processor::getVendor(vendor);
	vendor[12] = '\0';
	String::copy(data->cpuVendor, vendor);

	return currentThread;
}

/**
 * Returns the pci devide count
 */
SYSCALL_HANDLER(getPciDeviceCount)
{
	SyscallGetPciDeviceCount *data = (SyscallGetPciDeviceCount*) SYSCALL_DATA(currentThread->cpuState);
	data->count = Pci::getDeviceCount();
	return currentThread;
}

/**
 * Returns the pci device descriptor by the id provided
 */
SYSCALL_HANDLER(getPciDevice)
{
	PciDeviceHeader *data = (PciDeviceHeader*) SYSCALL_DATA(currentThread->cpuState);
	PciHeader *pciHeader = Pci::getDeviceAt(data->position);
	data->found = false;

	// only if header is avaible
	if (pciHeader)
	{
		data->found = true;

		data->bus = pciHeader->bus;
		data->slot = pciHeader->slot;
		data->function = pciHeader->function;

		data->vendorId = pciHeader->vendorId;
		data->deviceId = pciHeader->deviceId;

		data->classCode = pciHeader->classCode;
		data->subclassCode = pciHeader->subclassCode;
		data->progIf = pciHeader->progIf;
	}

	return currentThread;
}

/**
 * Returns the Thread count
 */
SYSCALL_HANDLER(getThreadCount)
{
	SyscallGetThreadCount *data = (SyscallGetThreadCount*) SYSCALL_DATA(currentThread->cpuState);
	data->count = Tasking::count(data->type);
	return currentThread;
}

/**
 * Gets the Thread pid list
 */
SYSCALL_HANDLER(getThreadIDs)
{
	SyscallGetThreadIDs *data = (SyscallGetThreadIDs*) SYSCALL_DATA(currentThread->cpuState);
	data->filledIds = Tasking::getTaskIDs(data->idBuffer, data->idBufferSize, data->type);
	return currentThread;
}

/**
 * Gets the Thread descriptor by it's tid
 */
SYSCALL_HANDLER(getThreadDescriptor)
{
	ThreadDescriptor *data = (ThreadDescriptor*) SYSCALL_DATA(currentThread->cpuState);
	Thread *thread = Tasking::getTaskById(data->id);

	// check validity
	if (thread)
	{
		// set the parent process
		data->found = true;
		if (thread->process->main) data->parent = thread->process->main->id;
		else data->parent = -1;

		// set the type
		data->type = thread->type;

		// provide the stack size
		data->threadUserStackSize = ThreadManager::getThreadStackSize(thread);

		// copy identifier
		const char *threadName = thread->getIdentifier();
		if (threadName) String::copy(data->identifier, threadName);
		else data->identifier[0] = '\0';

		// provide infos about rounds wait and scheduled times
		data->waitCount = thread->waitCount;
		data->executionTime = thread->rounds * APIC_MILLISECONDS_PER_TICK;
		data->scheduledTimes = thread->rounds;
	}

	else data->found = false;

	return currentThread;
}

/**
 * Returns the process count
 */
SYSCALL_HANDLER(getProcessCount)
{
	SyscallGetProcessCount *data = (SyscallGetProcessCount*) SYSCALL_DATA(currentThread->cpuState);
	data->count = Tasking::count(THREAD_TYPE_MAIN | THREAD_TYPE_VM86);
	return currentThread;
}

/**
 * Gets the process pid list
 */
SYSCALL_HANDLER(getProcessIDs)
{
	SyscallGetProcessIDs *data = (SyscallGetProcessIDs*) SYSCALL_DATA(currentThread->cpuState);
	data->filledIds = Tasking::getTaskIDs(data->idBuffer, data->idBufferSize, THREAD_TYPE_MAIN | THREAD_TYPE_VM86);
	return currentThread;
}

/**
 * Gets the process descriptor by it's pid
 */
SYSCALL_HANDLER(getProcessDescriptor)
{
	ProcessDescriptor *data = (ProcessDescriptor*) SYSCALL_DATA(currentThread->cpuState);
	Thread *main = Tasking::getTaskById(data->main.id);

	// check validity
	if (main)
	{
		// set the parent process
		data->main.found = true;
		if (main->process->parent) data->main.parent = main->process->parent->main->id;
		else data->main.parent = -1;

		// set the type
		data->main.type = main->type;

		// only for main or vm86 provide process size informations
		data->heapUsed = ThreadManager::getProcessHeapSize(main);
		data->imageSize = ThreadManager::getProcessImageSize(main);

		// copy identifier
		const char *name = main->getIdentifier();
		if (name) String::copy(data->main.identifier, name);
		else data->main.identifier[0] = '\0';

		// copy process source
		char *sourcePath = main->process->sourcePath;
		if (sourcePath) String::copy(data->sourcePath, sourcePath);
		else data->sourcePath[0] = '\0';

		// provide infos about rounds wait and scheduled times
		data->main.waitCount = main->waitCount;
		data->main.executionTime = main->rounds * APIC_MILLISECONDS_PER_TICK;
		data->main.scheduledTimes = main->rounds;
	}

	else data->main.found = false;

	return currentThread;
}
