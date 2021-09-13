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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef __LIBAPI_INTERNAL__
#define __LIBAPI_INTERNAL__

#include "eva.h"

__BEGIN_C

/**
 * called when a thread function provided returns
 */
void __ExitThread();

/**
 * The address of this function is inserted as the return address for signal & irq handlers.
 * It does nothing but calling the <RestoreInterruptedState> function.
 */
void __RestoreInterruptedStateCallback();

/**
 * Common atomic interface used by atomic*ock calls
 *
 * @param atom:         the atom to use
 * @param atom2:        the second atom to use
 * @param setOnFinish:  tell to kernel to set atoms when operations finish
 * @param tryOnly:      tell to kernel to try atomic wait
 *
 * @security-level APPLICATION
 */
uint8_t __AtomicLock(uint8_t* atom1, uint8_t* atom2, bool setOnFinish, bool tryOnly);

/**
 * copy source to destination to at least length bytes
 *
 * @param destination:      pointer to memory copy destination
 * @param source:           the source to copy on destination
 * @param length:           the size of the source
 * @return the destination pointer
 */
void* memcopy(void* destination, const void* source, size_t length);

/**
 * returns the length of the provided string
 *
 * @param str:      the string that we measure
 * @return the length of the string
 */
size_t strlength(const char* str);

__END_C

#endif
