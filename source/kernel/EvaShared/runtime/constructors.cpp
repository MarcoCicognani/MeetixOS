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

#include <runtime/constructors.hpp>
#include <logger/logger.hpp>

/**
 * Definition of a constructor
 */
typedef void (*Constructor)();

/**
 * Constructor symbols, defined in the kernel linker script. Please note that these
 * variables are not actually pointers, but symbols of no type (use "&" to get
 * their address).
 */
extern "C"
{
	void *startConstructors;
	void *endConstructors;
}

/**
 * call each defined constructor
 */
void Constructors::call()
{
	// create simil iterators
	Constructor *start = (Constructor*) &startConstructors;
	Constructor *end = (Constructor*) &endConstructors;

	// call each constructor
	for (Constructor *current = start; current != end; ++current)
		(*current)();
}
