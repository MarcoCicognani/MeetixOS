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

#include "__internal.h"
#include "eva/user.h"

/**
 * Trys to perform atomic wait. If the lock is already locked, the function
 * returns 0. Otherwise, the lock is set as in {AtomicLock} and the
 * function returns 1.
 *
 * @param atom:         the atom to use
 *
 * @security-level APPLICATION
 */
uint8_t AtomicTryLock(uint8_t *atom)
{
	return __AtomicLock(atom, NULL, true, true);
}

/**
 * Trys to perform atomic wait. If the lock is already locked, the function
 * returns 0. Otherwise, the lock is set as in {AtomicLock} and the
 * function returns 1.
 *
 * @param atom:         the atom to use
 * @param atom2:        the second atom to use
 *
 * @security-level APPLICATION
 */
uint8_t AtomicTryLockDual(uint8_t *atom1, uint8_t *atom2)
{
	return __AtomicLock(atom1, atom2, true, true);
}
