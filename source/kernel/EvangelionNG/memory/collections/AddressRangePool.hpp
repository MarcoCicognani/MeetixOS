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

#ifndef EVA_MEMORY_ADDRESS_RANGE_POOL
#define EVA_MEMORY_ADDRESS_RANGE_POOL

#include <memory/memory.hpp>

/**
 * An address range is a range of pages starting at a base. The base
 * determines the address of the range, and pages is the number of
 * pages the range has.
 */
struct AddressRange
{
public:
	/**
	 * constructor
	 */
	AddressRange() : next(0), used(false), base(0), pages(0), flags(0) {}

	/**
	 * linkage pointer
	 */
	AddressRange *next;  // next AddressRange object

	/**
	 * memory informations
	 */
	bool used;      // flag for used
	Address base;   // address of range
	uint32_t pages; // used pages

	/**
	 * permissions
	 */
	uint8_t flags; // permission flag
};

/**
 * A address range pool manages ranges of page-aligned virtual
 * addresses.
 */
class AddressRangePool
{
private:
	/**
	 * list of addresses
	 */
	AddressRange *first;

public:
	/**
	 * constructor
	 */
	AddressRangePool() : first(0) {}

	/**
	 * destructor
	 */
	~AddressRangePool();

	/**
	 * Inserts the addresses in the given range as free addresses in the pool.
	 * The start and end should be page-aligned.
	 *
	 * @param start:		the start address
	 * @param end:			the end address
	 */
	void initialize(Address start, Address end);

	/**
	 * Initialize by clone
	 *
	 * @param other:	another AddressRangePool object
	 */
	void initialize(AddressRangePool *other);

	/**
	 * allocate a contiguos count pages of memory with flags
	 *
	 * @param pages:		number of pages to allocate
	 * @param-opt flag:		optional code to assign to the allocated memory
	 */
	Address allocate(uint32_t pages, uint8_t flags = 0);

	/**
	 * frees an address and perform a merge and return freed pages
	 *
	 * @param base:		the address to frees
	 * @return number of frees pages
	 */
	int32_t free(Address base);

	/**
	 * @return the list of AddressRange
	 */
	AddressRange *getRanges();

	/**
	 * Clears the ranges
	 */
	void clear();

	/**
	 * debug
	 *
	 * @param-opt onlyFree:		show only free pages
	 */
	void dump(bool onlyFree = false);

private:
	/**
	 * merges contiguous free ranges
	 */
	void merge();
};

#endif
