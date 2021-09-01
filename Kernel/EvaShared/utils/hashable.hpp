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

#ifndef EVA_SHARED_UTILS_HASHCODE
#define EVA_SHARED_UTILS_HASHCODE

#include "eva/stdint.h"

/**
 * class to generate hash code from different types
 */
class HashAble
{
public:
	/**
	 * destructor
	 */
	virtual ~HashAble() {}

	/**
	 * generate hash code from a template pointer
	 *
	 * @template T:		the template type parameter
	 * @param p:		the T pointer to get hash code to
	 * @return the hash code of the pointer
	 */
	template<typename T> uint32_t hashcode(const T *p) { return (uint64_t) p; }

	/**
	 * get the hash code from another HashAble object
	 *
	 * @param value:	the HashAble object to get value to
	 * @return the hash code of the value into the HashAble object
	 */
	static uint32_t hashcode(const HashAble &value) { return value.hashcode(); }

	/**
	 * get the hash code from an unsigned long value
	 *
	 * @param value:	the value to get the hash code to
	 * @return the hash code value
	 */
	static uint32_t hashcode(uint64_t value) { return value; }

	/**
	 * interface method that must be redefined
	 */
	virtual uint32_t hashcode() const = 0;
};

#endif
