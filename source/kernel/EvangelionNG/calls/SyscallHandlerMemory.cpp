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

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <executable/Elf32Loader.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/physical/PPreferenceTracker.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/TemporaryPagingUtil.hpp>
#include <memory/constants.hpp>
#include <memory/LowerHeap.hpp>

/**
 * Allocates a memory area of at least "size" bytes. Memory is always allocated page-wise,
 * therefore the area is always page-aligned and has a size of a multiple of the page size.
 *
 * Allocating memory using this call makes the requesting process the physical owner of the
 * pages in its virtual space (important for unmapping).
 */
SYSCALL_HANDLER(allocMem)
{
	Process *process = currentThread->process;

	SyscallAllocMem *data = (SyscallAllocMem*) SYSCALL_DATA(currentThread->cpuState);
	data->virtualResult = 0;

	// Get the number of pages
	uint32_t pages = PAGE_ALIGN_UP(data->size) / PAGE_SIZE;
	if (pages > 0)
	{
		// Allocate a virtual range, we are physical owner
		uint8_t virtualRangeFlags = PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER;
		VirtualAddress virtualRangeBase = process->virtualRanges.allocate(pages, virtualRangeFlags);

		if (virtualRangeBase != 0)
		{
			bool mappingSuccessful = true;

			// Perform either page-wise mapping
			for (uint32_t i = 0; i < pages; i++)
			{
				PhysicalAddress addr = PPallocator::allocate();
				if (!addr)
				{
					mappingSuccessful = false;
					break;
				}

				AddressSpace::map(virtualRangeBase + i * PAGE_SIZE, addr, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
			}

			// Check if the mapping was successful
			if (mappingSuccessful)
			{
				// Successful, return the range base
				data->virtualResult = (void*) virtualRangeBase;

				logDebug("%! allocated memory area of size %h at virt %h for process %i", "syscall", pages * PAGE_SIZE, data->virtualResult, process->main->id);
			}

			else
			{
				// Failed to create the area, free everything
				logWarn("%! went out of memory during shared memory allocation for process %i, rollback...", "syscall", process->main->id);
				for (uint32_t i = 0; i < pages; i++)
				{
					PhysicalAddress addr = AddressSpace::virtualToPhysical(virtualRangeBase + i * PAGE_SIZE);
					if (addr) PPallocator::free(addr);
				}
				process->virtualRanges.free(virtualRangeBase);
			}
		}
	}

	return currentThread;
}

/**
 * Shares the memory area at "memory" with a size of "size" with the process that has the
 * identifier "processID". The "virtualAddress" contains the virtual address of the area
 * in the target processes address space if the sharing was successful. Memory is always
 * shared page-wise. The resulting virtual address in the target space is always
 * page-aligned.
 *
 * The area to share should always is always page-wise and the given address must be
 * page-aligned, thus only memory that was allocated page-wise should be shared.
 *
 * The target process then is not the physical owner of the shared pages.
 */
SYSCALL_HANDLER(shareMem)
{
	SyscallShareMem *data = (SyscallShareMem*) SYSCALL_DATA(currentThread->cpuState);
	data->virtualAddress = 0;

	// Get the number of pages
	uint32_t memory = (uint32_t) data->memory;
	uint32_t pages = PAGE_ALIGN_UP(data->size) / PAGE_SIZE;

	// Only allow sharing in user memory
	if (memory < CONST_KERNEL_AREA_START && (memory + pages * PAGE_SIZE) <= CONST_KERNEL_AREA_START)
	{
		// Get the target process
		Thread *targetThread = Tasking::getTaskById(data->processID);
		if (targetThread)
		{
			Process *targetProcess = targetThread->process;

			// Get the range in the other process space
			VirtualAddress virtualRangeBase = targetProcess->virtualRanges.allocate(pages, PROC_VIRTUAL_RANGE_FLAG_NONE);
			if (virtualRangeBase != 0)
			{
				PageDirectory executingSpace = AddressSpace::getCurrentSpace();

				// Map the pages to the other processes space
				for (uint32_t i = 0; i < pages; i++)
				{
					PhysicalAddress physicalAddr = AddressSpace::virtualToPhysical(memory + i * PAGE_SIZE);

					AddressSpace::switchToSpace(targetProcess->pageDirectory);
					AddressSpace::map(virtualRangeBase + i * PAGE_SIZE, physicalAddr, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
					AddressSpace::switchToSpace(executingSpace);
				}

				// Done
				data->virtualAddress = (void*) virtualRangeBase;

				logDebug("%! shared memory area of process %i at %h of size %h with process %i to address %h", "syscall", currentThread->process->main->id, memory, pages * PAGE_SIZE, targetProcess->main->id, virtualRangeBase);
			}

			else
			{
				logWarn("%! process %i was unable to share memory are %h of size %h with process %i because there was no virtual range", "syscall", currentThread->process->main->id, memory, pages * PAGE_SIZE, targetProcess->main->id);
			}
		}

		else
		{
			logWarn("%! process %i was unable to share memory with the other process because it was null", "syscall", currentThread->process->main->id);
		}
	}

	else
	{
		logWarn("%! process %i was unable to share memory because addresses below %h are not allowed", "syscall", currentThread->process->main->id, CONST_KERNEL_AREA_START);
	}

	return currentThread;

}

/**
 * Maps the physical area at "physicalAddress" with a size of at least "size" to the
 * given "virtualAddress" in the current address space. Mapping is always done page-wise.
 *
 * This does not make the current process the physical owner of the physical pages,
 * as they may possibly not be used otherwise once the process unmaps them.
 */
SYSCALL_HANDLER(mapMmio)
{
	Process *process = currentThread->process;

	SyscallMapMmio *data = (SyscallMapMmio*) SYSCALL_DATA(currentThread->cpuState);
	data->virtualAddress = 0;

	// Only kernel and drivers may do this
	if (process->securityLevel <= SECURITY_LEVEL_DRIVER)
	{
		uint32_t pages = PAGE_ALIGN_UP(data->size) / PAGE_SIZE;

		// Allocate a virtual range (but not be physical owner)
		VirtualAddress range = process->virtualRanges.allocate(pages, PROC_VIRTUAL_RANGE_FLAG_NONE);
		uint32_t physical = (uint32_t) data->physicalAddress;
		if (pages > 0 && range != 0)
		{
			// Map the pages to the space
			for (uint32_t i = 0; i < pages; i++)
				AddressSpace::map(range + i * PAGE_SIZE, physical + i * PAGE_SIZE, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);

			data->virtualAddress = (void*) range;
		}
	}

	return currentThread;
}

/**
 * Unmaps the area at the given "virtualBase". This address must be page-aligned.
 *
 * If the current process is the physical owner of the pages within this area,
 * these physical pages are freed.
 */
SYSCALL_HANDLER(unmap)
{
	Process *process = currentThread->process;

	SyscallUnmap *data = (SyscallUnmap*) SYSCALL_DATA(currentThread->cpuState);
	VirtualAddress base = data->virtualBase;

	// Search for the range
	AddressRange *range = process->virtualRanges.getRanges();
	while (range)
	{
		if (range->base == base) break;
		range = range->next;
	}

	// Found range, free it
	if (range)
	{
		// If physical owner, free physical range
		if (range->flags & PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER)
			for (uint32_t i = 0; i < range->pages; i++)
				PPallocator::free(AddressSpace::virtualToPhysical(range->base + i * PAGE_SIZE));

		// Unmap pages
		for (uint32_t i = 0; i < range->pages; i++)
			AddressSpace::unmap(range->base + i * PAGE_SIZE);

		// Free the virtual range
		process->virtualRanges.free(range->base);
		logDebug("%! task %i in process %i unmapped range at %h", "syscall", process->main->id, currentThread->id, base);
	}

	else
	{
		logWarn("%! task %i in process %i tried to unmap range at %h that was never mapped", "syscall", process->main->id, currentThread->id, base);
	}

	return currentThread;
}

/**
 * Increases/decreases the program heap break of the current process.
 */
SYSCALL_HANDLER(sbrk)
{
	Process *process = currentThread->process;
	SyscallSbrk *data = (SyscallSbrk*) SYSCALL_DATA(currentThread->cpuState);

	// initialize the heap if necessary
	if (!process->heapBreak)
	{
		VirtualAddress heapStart = process->imageEnd;

		PhysicalAddress phys = PPallocator::allocate();
		AddressSpace::map(heapStart, phys, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
		PPreferenceTracker::increment(phys);

		process->heapBreak = heapStart;
		process->heapStart = heapStart;
		process->heapPages = 1;
		logDebug("%! process %i initializes his heap at %h", "syscall", process->main->id, heapStart);
	}

	// calculate new address
	VirtualAddress brkOld = process->heapBreak;
	VirtualAddress brkNew = brkOld + data->amount;

	// heap expansion is limited
	if (brkNew >= CONST_USER_MAXIMUM_HEAP_BREAK)
	{
		logInfo("%! process %i went out of memory when setting heap break", "syscall", process->main->id);
		data->address = -1;
		data->successful = false;
	}

	else
	{
		// expand if necessary
		VirtualAddress virtAbove;
		while (brkNew > (virtAbove = process->heapStart + process->heapPages * PAGE_SIZE))
		{
			PhysicalAddress phys = PPallocator::allocate();
			AddressSpace::map(virtAbove, phys, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
			PPreferenceTracker::increment(phys);
			++process->heapPages;
		}

		// shrink if possible
		VirtualAddress virtBelow;
		while (brkNew < (virtBelow = process->heapStart + process->heapPages * PAGE_SIZE - PAGE_SIZE))
		{
			PhysicalAddress phys = AddressSpace::virtualToPhysical(virtBelow);
			AddressSpace::unmap(virtBelow);
			if (!PPreferenceTracker::decrement(phys)) PPallocator::free(phys);
			--process->heapPages;
		}

		process->heapBreak = brkNew;
		data->address = brkOld;
		data->successful = true;
	}

	logDebug("%! <%i> sbrk(%i): %h -> %h (%h -> %h, %i pages)", "syscall", process->main->id, data->amount, data->address, process->heapBreak, process->heapStart, process->heapStart + process->heapPages * PAGE_SIZE, process->heapPages);

	return currentThread;
}

/**
 * Allocates a memory chunk in the area below 1MiB. These memory areas can then
 * be used for calling interrupts using the virtual 8086 mode.
 */
SYSCALL_HANDLER(lowerMalloc)
{
	Process *process = currentThread->process;
	SyscallLowerMalloc *data = (SyscallLowerMalloc*) SYSCALL_DATA(currentThread->cpuState);

	// check the security
	if (process->securityLevel <= SECURITY_LEVEL_DRIVER) data->result = LowerHeap::allocate(data->size);
	else
	{
		logDebug("%! task %i tried to allocate lower memory but does not have permission", "syscall", currentThread->id);
		data->result = 0;
	}

	return currentThread;
}

/**
 * Frees a memory chunk that was allocated using the lower memory allocation call.
 */
SYSCALL_HANDLER(lowerFree)
{
	Process *process = currentThread->process;
	SyscallLowerFree *data = (SyscallLowerFree*) SYSCALL_DATA(currentThread->cpuState);

	if (process->securityLevel <= SECURITY_LEVEL_DRIVER) LowerHeap::free(data->memory);
	else
	{
		logDebug("%! task %i tried to free lower memory but does not have permission", "syscall", currentThread->id);
	}
	return currentThread;
}
