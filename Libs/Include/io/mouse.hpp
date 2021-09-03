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

#ifndef MEETIX_LIBRARY_IO_MOUSE
#define MEETIX_LIBRARY_IO_MOUSE

#include <stdint.h>
#include <string>
#include <sstream>

/**
 * static class to get mouse informations
 */
class Mouse
{
private:
	/**
	 * remove constructor
	 */
	Mouse() = delete;

public:
	/**
	 * mouse info descriptor
	 */
	struct Info
	{
	public:
		/**
		 * empty constructor
		 */
		Info() :
			x(0), y(0), button1(false),
			button2(false), button3(false) {}

		/**
		 * filled constructor
		 *
		 * @param x:		the x value
		 * @param y:		the y value
		 * @param btn1:		the button 1 value
		 * @param btn2:		the button 2 value
		 * @param btn3:		the button 3 value
		 */
		Info(int16_t x, int16_t y, bool btn1, bool btn2, bool btn3) :
			x(x), y(y), button1(btn1),
			button2(btn2), button3(btn3) {}

		/**
		 * position data
		 */
		int16_t x;
		int16_t y;

		/**
		 * pressed button data
		 */
		bool button1;
		bool button2;
		bool button3;
	};

	/**
	 * @return an instance of mouse info from the shared area of ps2 driver
	 */
	static Info read();
};

#endif
