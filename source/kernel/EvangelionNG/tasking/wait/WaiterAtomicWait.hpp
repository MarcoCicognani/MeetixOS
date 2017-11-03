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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_ATOMIC_WAIT
#define EVA_MULTITASKING_WAIT_MANAGER_ATOMIC_WAIT

#include <tasking/wait/waiter.hpp>
#include <logger/logger.hpp>

/**
 * waiter implementation used for atomic syncronization
 */
class WaiterAtomicWait : public Waiter
{
private:
	/**
	 * internal data
	 */
	uint8_t *atom1;		// the first atom
	uint8_t *atom2;		// the second atom (non always provided)
	bool setOnFinish;	// flag used for the {AtomicLock} syscall

public:
	/**
	 * filled constructor
	 *
	 * @param atom1:			the pointer to the first atom to use
	 * @param atom2:			the pointer to the second atom to use
	 * @param setOnFinish:		tell to waiter if atoms are to change on finisching
	 */
	WaiterAtomicWait(uint8_t *atom1, uint8_t *atom2, bool setOnFinish) : atom1(atom1), atom2(atom2), setOnFinish(setOnFinish) {}

	/**
	 * implementation of check waiting method
	 *
	 * @param task:		the task that wait
	 * @return true if task must keep waiting
	 */
	virtual bool checkWaiting(Thread *task)
	{
		// need waiting?
		bool keepWaiting = *atom1 && (!atom2 || *atom2);

		// once waiting is finished, set the atom if required
		if (!keepWaiting && setOnFinish)
		{
			*atom1 = true;
			if (atom2) *atom2 = true;
		}

		return keepWaiting;
	}

	/**
	 * @return the name of the waiter
	 */
	virtual const char *debugName(){
		return "WaiterAtomicWait";
	}
};

#endif
