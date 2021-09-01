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

#include <eva.h>
#include <io/ps2.hpp>
#include <io/mouse.hpp>

/**
 * @return an instance of mouse info from the shared area of ps2 driver
 */
Mouse::Info Mouse::read()
{
	// register client if not registered
	if (!ps2IsRegistered)
		if (!Ps2::registerSelf()) return Info();

	// wait until incoming data is here (and the driver unsets the atom)
	AtomicBlock(&ps2Area->mouse.atomNothingQueued);

	// take info from the shared memory
	Info event;
	event.x = ps2Area->mouse.moveX;
	event.y = ps2Area->mouse.moveY;
	event.button1 = (ps2Area->mouse.flags & (1 << 0));
	event.button2 = (ps2Area->mouse.flags & (1 << 1));
	event.button3 = (ps2Area->mouse.flags & (1 << 2));

	// tell driver that we've handled it
	ps2Area->mouse.atomNothingQueued = true;
	ps2Area->mouse.atomUnhandled = false;

	return event;
}
