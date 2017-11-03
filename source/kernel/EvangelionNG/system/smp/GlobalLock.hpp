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

#ifndef EVA_SYSTEM_SMP_GLOBALLOCK
#define EVA_SYSTEM_SMP_GLOBALLOCK

#include "eva/stdint.h"

/**
 * amage system locker
 */
class GlobalLock
{
private:
	/**
	 * locker atom
	 */
	volatile int atom = 0;

public:
	/**
	 * lock the system
	 */
	void lock();

	/**
	 * unlock the system
	 */
	void unlock();

	/**
	 * check locking
	 *
	 * @return wether is locking
	 */
	bool isLocked();

	/**
	 * set the atom value
	 *
	 * @param val:	 the value to set to atom
	 */
	void quiet(int val);
};

#endif
