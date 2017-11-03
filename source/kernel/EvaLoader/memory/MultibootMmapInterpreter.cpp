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

#include <memory/MultibootMmapInterpreter.hpp>
#include <multiboot/multiboot.hpp>
#include <logger/logger.hpp>
#include <loader.hpp>

/**
 * Reads the GRUB memory map to find out which memory areas are usable and free.
 * Excludes everything before "reservedAreaEnd" and also excludes the locations
 * of the multiboot modules.
 *
 * @param allocator:		the allocator object where mark free addresses
 * @param reservedAreaEnd:	the end address of the reserved area
 */
void MultibootMmapInterpreter::load(BitMapPageAllocator *allocator, uint32_t reservedAreaEnd)
{
	MultibootInformation *mbInfo = EvaLoader::getSetupInformation()->multibootInformation;

	MultibootMmap *map = (MultibootMmap*) mbInfo->memoryMapAddress;
	uint32_t mapListEnd = mbInfo->memoryMapAddress + mbInfo->memoryMapLength;

	// Iterate over the list of memory maps from GRUB
	logInfo("%! memory regions:", "memmap");
	while (((uint32_t) map) < mapListEnd)
	{
		// Check if the map is usable memory
		if (map->type == 1)
		{
			uint64_t areaStart = (uint64_t) map->baseAddressLower | ((uint64_t) map->baseAddressHigher << 32);
			uint64_t areaEnd = areaStart + ((uint64_t) map->lengthLower | ((uint64_t) map->lengthHigher << 32));

			// If this ranges is out of 32bit bounds, ignore it
			if (areaStart > 0xFFFFFFFF) logInfo("%# > 0xFFFFFFFF             : not usable");

			else
			{
				logInfon("%#   %h - %h", (uint32_t ) areaStart, (uint32_t ) areaEnd);

				// Make sure that the mapped area lays behind the kernel
				if (areaStart < reservedAreaEnd) areaStart = reservedAreaEnd;

				// End of area above 32bit? Cut off
				if (areaEnd > 0xFFFFFFFF) areaEnd = 0xFFFFFFFF;

				// Page-align
				areaStart = PAGE_ALIGN_UP(areaStart);
				areaEnd = PAGE_ALIGN_DOWN(areaEnd);

				// Mark as free
				uint32_t chunkCount = 0;
				uint32_t inModule = 0;

				if (areaEnd > areaStart)
				{
					// Split into page sized chunks
					while (areaStart < areaEnd - PAGE_SIZE)
					{
						// Exclude memory within modules
						bool isInModule = false;
						for (uint32_t i = 0; i < mbInfo->modulesCount; i++)
						{
							MultibootModule *module = (MultibootModule*) (mbInfo->modulesAddress + sizeof(MultibootModule) * i);

							if ((areaStart >= PAGE_ALIGN_DOWN(module->moduleStart)) && (areaStart < PAGE_ALIGN_UP(module->moduleEnd)))
							{
								isInModule = true;
								break;
							}
						}

						// If its not inside a module, mark as free
						if (isInModule) ++inModule;

						else
						{
							allocator->markFree(areaStart);
							++chunkCount;
						}

						areaStart = areaStart + PAGE_SIZE;
					}
				}

				logInfo(": %i available (%i blocked)", chunkCount, inModule);
			}
		}

		// Skip to the next map (the sizeof in the end is something GRUB-specific, look up the docs)
		map = (MultibootMmap*) ((uint32_t) map + map->size + sizeof(uint32_t));
	}
}
