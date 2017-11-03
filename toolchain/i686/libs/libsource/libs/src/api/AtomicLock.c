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
 * Performs an atomic wait. If the atom is true, the executing task must
 * wait until the task that owns the atom has finished its work and sets
 * it to false. Once the atom is false, it is set to true and the function
 * returns.
 *
 * @param atom:         the atom to use
 *
 * @security-level APPLICATION
 */
void AtomicLock(uint8_t *atom)
{
	__AtomicLock(atom, NULL, true, false);
}

/**
 * Performs an atomic wait. If the atom is true, the executing task must
 * wait until the task that owns the atom has finished its work and sets
 * it to false. Once the atom is false, it is set to true and the function
 * returns.
 *
 * @param atom:         the atom to use
 * @param atom2:        the second atom to use
 *
 * @security-level APPLICATION
 */
void AtomicLockDual(uint8_t *atom1, uint8_t *atom2)
{
	__AtomicLock(atom1, atom2, true, false);
}
