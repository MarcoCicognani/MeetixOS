/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
* 																			     *
* This program is free software; you can redistribute it and/or                  *
* modify it under the terms of the GNU General Public License                    *
* as published by the Free Software Foundation; either version 2				 *
* of the License, or (char *argumentat your option) any later version.			 *
*																				 *
* This program is distributed in the hope that it will be useful,				 *
* but WITHout ANY WARRANTY; without even the implied warranty of                 *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include "eva.h"
#include "eva/stdint.h"
#include "__internal.h"

/**
 * copy source to destination to at least length bytes
 *
 * @param destination:      pointer to memory copy destination
 * @param source:           the source to copy on destination
 * @param length:           the size of the source
 * @return the destination pointer
 */
void *memcopy(void *destination, const void *source, size_t length)
{
	uint8_t *bytesrc = (uint8_t*) source;
	uint8_t *bytedest = (uint8_t*) destination;
	while (length--)
		*bytedest++ = *bytesrc++;

	return destination;
}

/**
 * returns the length of the provided string
 *
 * @param str:      the string that we measure
 * @return the length of the string
 */
size_t strlength(const char *str)
{
	size_t length = 0;
	while (str[length++]);
	
	return length;
}
