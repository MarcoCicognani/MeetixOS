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

#include <memory/memory.hpp>

/**
 * Sets number bytes at target to value.
 *
 * @param target:		the target pointer
 * @param value:		the byte value
 * @param number:		the number of bytes to set
 * @return <target>
 */
void *Memory::setBytes(void *target, uint8_t value, int32_t length)
{
	uint8_t* pos = (uint8_t*) target;
	while (length--)
		*pos++ = (uint8_t) value;

	return target;
}

/**
 * Sets number words at target to value.
 *
 * @param target:	the target pointer
 * @param value:	the word value
 * @param number:	the number of word to set
 * @return <target>
 */
void *Memory::setWords(void *target, uint16_t value, int32_t length)
{
	uint16_t* pos = (uint16_t*) target;
	while (length--)
		*pos++ = (uint16_t) value;

	return target;
}

/**
 * Copys size bytes from source to target.
 *
 * @param source:	pointer to the source memory location
 * @param target:	pointer to the target memory location
 * @param size:		number of bytes to copy
 * @return <target>
 */
void *Memory::copy(void *target, const void *source, int32_t length)
{
	uint8_t *targetPos = (uint8_t*) target;
	const uint8_t *sourcePos = (const uint8_t*) source;
	while (length--)
		*targetPos++ = *sourcePos++;

	return target;
}
