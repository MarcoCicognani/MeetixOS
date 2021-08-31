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

#include <memory/physical/PPreferenceTracker.hpp>
#include <memory/paging.hpp>
#include <logger/logger.hpp>

PPreferenceCountDirectory directory;

/**
 * increment the references to the provided address
 *
 * @param address:		the new address to add
 */
void PPreferenceTracker::increment(PhysicalAddress address)
{
	uint32_t ti = TABLE_IN_DIRECTORY_INDEX(address);
	uint32_t pi = PAGE_IN_TABLE_INDEX(address);

	if (!directory.tables[ti])
	{
		directory.tables[ti] = new PPreferenceCountTable();

		for (uint32_t i = 0; i < 1024; i++)
			directory.tables[ti]->referenceCount[i] = 0;
	}

	++(directory.tables[ti]->referenceCount[pi]);
}

/**
 * decrement the references to the provided address
 *
 * @param address:		the physical address to be unreferenced
 * @return the number of references to the provided address
 */
int16_t PPreferenceTracker::decrement(PhysicalAddress address)
{
	uint32_t ti = TABLE_IN_DIRECTORY_INDEX(address);
	uint32_t pi = PAGE_IN_TABLE_INDEX(address);

	if (!directory.tables[ti]) return 0;
	return --(directory.tables[ti]->referenceCount[pi]);
}
