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

#ifndef EVA_SHARED_SYSTEM_SYSTEM
#define EVA_SHARED_SYSTEM_SYSTEM

#include <system/processor.hpp>
#include "eva/stdint.h"

/**
 * System class for processor and interrupt controller handling
 */
class System
{
public:

	/**
	 * Used on the BSP to initialize AP processors and interrupt controllers
	 *
	 * @param initialPageDirectoryPhysical:		physical address of the initial page
	 *							 				directory, used for AP startup
	 */
	static void initializeBasicSystemPackage(PhysicalAddress initialPageDirectoryPhysical);

	/**
	 * Used on the APs for initialization
	 */
	static void initializeAdvancedPackage();

	/**
	 * @return the processors descriptor list
	 */
	static Processor *getProcessorList();

	/**
	 * @return the number of cpus
	 */
	static uint32_t getNumberOfProcessors();

	/**
	 * @return the identifier to use for core identification
	 */
	static uint32_t currentProcessorId();

	/**
	 * Creates a new processor descriptor with the provided identifier
	 *
	 * @param apicId:		the apic id of the cpu
	 */
	static void addProcessor(uint32_t apicId);

	/**
	 * return the cpu descriptor from provided id
	 *
	 * @return the cpu descriptor
	 */
	static Processor *getProcessorById(uint32_t coreId);

	/**
	 * check and enable the SSE/SSE2 instructions set
	 */
	static void checkAndEnableSSE();

};

#endif
