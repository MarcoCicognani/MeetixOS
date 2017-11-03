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

#ifndef EVA_SHARED_MEMORY_MEMORY
#define EVA_SHARED_MEMORY_MEMORY

#include "eva/types.h"
#include <stddef.h>
#include "eva/stdint.h"

/**
 * aligner macro, permits to align a memory segment to the up
 */
#define ALIGN_UP(value, alignment) 	((value % alignment) ? (value + (alignment - value % alignment)) : value)

/**
 * Class for memory manipulation
 */
class Memory
{
public:

	/**
	 * Sets number bytes at target to value.
	 *
	 * @param target:		the target pointer
	 * @param value:		the byte value
	 * @param number:		the number of bytes to set
	 * @return <target>
	 */
	static void *setBytes(void *target, uint8_t value, int32_t number);

	/**
	 * Sets number words at target to value.
	 *
	 * @param target:	the target pointer
	 * @param value:	the word value
	 * @param number:	the number of word to set
	 * @return <target>
	 */
	static void *setWords(void *target, uint16_t value, int32_t number);

	/**
	 * Copys size bytes from source to target.
	 *
	 * @param source:	pointer to the source memory location
	 * @param target:	pointer to the target memory location
	 * @param size:		number of bytes to copy
	 * @return <target>
	 */
	static void *copy(void *target, const void *source, int32_t size);

};

#endif
