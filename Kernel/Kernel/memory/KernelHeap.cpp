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

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/allocators/ChunkAllocator.hpp>
#include <memory/constants.hpp>
#include <memory/KernelHeap.hpp>
#include <memory/paging.hpp>
#include <memory/physical/PPallocator.hpp>

/**
 * new implementation
 */
void* operator new(usize size) {
    return KernelHeap::allocate(size);
}

/**
 * new[] implementation
 */
void* operator new[](usize size) {
    return KernelHeap::allocate(size);
}

/**
 * delete implementation
 */
void operator delete(void* m) {
    KernelHeap::free(m);
}

void operator delete(void* m, usize) {
    KernelHeap::free(m);
}

/**
 * delete[] implementation
 */
void operator delete[](void* m) {
    KernelHeap::free(m);
}

/**
 * allocated memory collector
 */
static ChunkAllocator allocator;

/**
 * heap ranges
 */
static VirtAddr heapStart        = 0;
static VirtAddr heapEnd          = 0;
static uint32_t usedMemoryAmount = 0;

/**
 * ready flag
 */
static bool kernelHeapInitialized = false;

/**
 * Initializes the kernel heap using the given range of memory.
 *
 * @param start:	the start address
 * @param end:		the end address
 */
void KernelHeap::initialize(VirtAddr start, VirtAddr end) {
    // initialize the collector and save the ranges
    allocator.initialize(start, end);
    heapStart = start;
    heapEnd   = end;

    // now we are ready to allocate the world!!
    logDebug("%! initialized with area: %h - %h", "kernheap", start, end);
    kernelHeapInitialized = true;
}

/**
 * Expands the heap space by {KERNEL_HEAP_EXPAND_STEP} bytes.
 *
 * @return whether the operation was successful
 */
bool KernelHeap::expandHeap() {
    // check limit
    if ( heapEnd > CONST_KERNEL_HEAP_MAXIMUM_END ) {
        logDebug("%! maximum reached during expansion", "kernheap");
        return false;
    }

    // Expand virtual space
    for ( VirtAddr virt = heapEnd; virt < heapEnd + KERNEL_HEAP_EXPAND_STEP;
          virt          = virt + PAGE_SIZE ) {
        // get a new physical page
        PhysAddr page = PPallocator::allocate();
        if ( !page ) {
            logWarn("%! no pages left for expanding", "kernheap");
            return false;
        }

        // map the page
        AddressSpace::map(virt, page, DEFAULT_KERNEL_TABLE_FLAGS, DEFAULT_KERNEL_PAGE_FLAGS);
    }

    // Create header
    allocator.expand(KERNEL_HEAP_EXPAND_STEP);
    heapEnd = heapEnd + KERNEL_HEAP_EXPAND_STEP;

    logDebug("%! expanded to end %h (%ikb in use)", "kernheap", heapEnd, usedMemoryAmount / 1024);
    return true;
}

/**
 * allocate a pointer of size on the kernel heap
 *
 * @param size:		size of the space to be allocated
 * @return the allocated pointer
 */
void* KernelHeap::allocate(uint32_t size) {
    // check ready flag
    if ( !kernelHeapInitialized )
        EvaKernel::panic("%! tried to use uninitialized kernel heap", "kernheap");

    // allocate with collector
    void* allocated = allocator.allocate(size);

    // check success
    if ( allocated ) {
        usedMemoryAmount = usedMemoryAmount + size;
        return allocated;
    }

    // no memory avaible, expand the heap
    else if ( expandHeap() )
        return allocate(size);

    // memory avaible for expansion, fail
    EvaKernel::panic("%! could not expand kernel heap", "kernheap");
    return 0;
}

/**
 * frees a memory pointer
 *
 * @param memory:		the pointer that be deallocated
 */
void KernelHeap::free(void* mem) {
    // check ready flag
    if ( !kernelHeapInitialized )
        EvaKernel::panic("%! tried to use uninitialized kernel heap", "kernheap");

    // frees the memory
    usedMemoryAmount = usedMemoryAmount - allocator.free(mem);
}

/**
 * @return the amount of used heap memory
 */
uint32_t KernelHeap::getUsedMemoryAmount() {
    return usedMemoryAmount;
}
