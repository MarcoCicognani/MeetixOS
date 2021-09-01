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

#include <multiboot/MultibootModuleAnalyzer.hpp>

#include <loader.hpp>
#include <logger/logger.hpp>

/**
 * Calculates the end of the highest module in memory.
 *
 * @return the address of the highest module in memory
 */
uint32_t MultibootModuleAnalyzer::calculateEndOfHighestModule()
{
	logDebug("%! searching for modules", "multiboot");
	uint32_t highestEnd = 0;

	// Check if modules are available
	MultibootInformation *mbInfo = EvaLoader::getSetupInformation()->multibootInformation;
	if (mbInfo->flags & (1 << 3))
	{
		uint32_t moduleCount = mbInfo->modulesCount;
		for (uint32_t i = 0; i < moduleCount; i++)
		{
			MultibootModule *module = (MultibootModule*) (mbInfo->modulesAddress + sizeof(MultibootModule) * i);
			logDebug("%#   %h - %h: %s ", module->moduleStart, module->moduleEnd, module->path);

			// Check modules end address
			if (module->moduleEnd > highestEnd) highestEnd = module->moduleEnd;
		}

	}

	else EvaLoader::panic("%! modules are not available", "multiboot");

	logDebug("%! highest module end address is %h", "multiboot", highestEnd);
	return highestEnd;
}
