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

#include <memory/gdt/GdtManager.hpp>
#include <memory/gdt/GdtMounter.hpp>
#include <memory/gdt/tss.hpp>
#include <memory/constants.hpp>
#include <system/system.hpp>
#include <logger/logger.hpp>
#include <system/smp/GlobalLock.hpp>

/**
 * global list of gdt entries
 */
static GdtListEntry **gdtList;

/**
 * Creates the array of GDT-to-core assignment
 * structs. Only called by the BSP.
 */
void GdtManager::prepare()
{
	// Create enough space for each core
	uint32_t numCores = System::getNumberOfProcessors();
	gdtList = new GdtListEntry*[numCores];

	// Create one GDT per core
	for (uint32_t i = 0; i < numCores; i++)
		gdtList[i] = new GdtListEntry();
}

/**
 * Initializes the local GDT.
 */
void GdtManager::initialize()
{
	// Initialize local GDT
	GdtListEntry *thisGdt = gdtList[System::currentProcessorId()];

	// Create the GDT pointer
	thisGdt->ptr.limit = (sizeof(GdtEntry) * GDT_NUM_ENTRIES) - 1;
	thisGdt->ptr.base = (uint32_t) &thisGdt->entry;

	// Null descriptor, position 0x00
	Gdt::createGate(&thisGdt->entry[0], 0, 0, 0, 0);

	// Kernel code segment descriptor, position 0x08
	Gdt::createGate(&thisGdt->entry[1], 0, 0xFFFFFFFF, ACCESS_BYTE__KERNEL_CODE_SEGMENT, 0xCF);

	// Kernel data segment descriptor, position 0x10
	Gdt::createGate(&thisGdt->entry[2], 0, 0xFFFFFFFF, ACCESS_BYTE__KERNEL_DATA_SEGMENT, 0xCF);

	// User code segment descriptor, position 0x18
	Gdt::createGate(&thisGdt->entry[3], 0, 0xFFFFFFFF, ACCESS_BYTE__USER_CODE_SEGMENT, 0xCF);

	// User data segment descriptor, position 0x20
	Gdt::createGate(&thisGdt->entry[4], 0, 0xFFFFFFFF, ACCESS_BYTE__USER_DATA_SEGMENT, 0xCF);

	// TSS descriptor, position 0x28
	Gdt::createGate(&thisGdt->entry[5], (uint32_t) &thisGdt->tss, sizeof(Tss), ACCESS_BYTE__TSS_386_SEGMENT, 0x40);
	thisGdt->tss.ss0 = GDT_DESCRIPTOR_KERNEL_DATA; // Kernel data segment
	thisGdt->tss.esp0 = 0; // will later be initialized by the scheduler

	// User thread pointer segment 0x30
	Gdt::createGate(&thisGdt->entry[6], 0, 0xFFFFFFFF, ACCESS_BYTE__USER_DATA_SEGMENT, 0xCF);

	// Load GDT
	logDebug("%! BSP descriptor table lays at %h", "gdt", &thisGdt->entry);
	logDebug("%! pointer lays at %h, base %h, limit %h", "gdt", &thisGdt->ptr, thisGdt->ptr.base, thisGdt->ptr.limit);
	_loadGdt((uint32_t) &thisGdt->ptr);
	logDebug("%! initialized", "gdt");

	// Load TSS
	logDebug("%! descriptor index %h", "tss", GDT_DESCRIPTOR_TSS);
	_loadTss(GDT_DESCRIPTOR_TSS);
	logDebug("%! initialized", "tss");
}

/**
 * Sets the ESP0 in the TSS. This is necessary for the system to know which stack
 * to use when switching from ring 3 to ring 0.
 *
 * @param esp0:		the address of the esp to set
 */
void GdtManager::setTssEsp0(VirtualAddress esp0)
{
	gdtList[System::currentProcessorId()]->tss.esp0 = esp0;
}

/**
 * set the user thread address from the provided address
 *
 * @param userThreadAddr:		the new user thread address
 */
void GdtManager::setUserThreadAddress(VirtualAddress userThreadAddr)
{
	GdtListEntry *listEntry = gdtList[System::currentProcessorId()];
	Gdt::createGate(&listEntry->entry[6], userThreadAddr, 0xFFFFFFFF, ACCESS_BYTE__USER_DATA_SEGMENT, 0xCF);
}
