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

#include <components/screen.hpp>

/**
 *
 */
void Screen_t::markDirty(Rectangle rect)
{

	// Mark area as invalid
	if (invalid.x == 0 && invalid.y == 0 && invalid.width == 0 && invalid.height == 0) invalid = rect;

	else
	{
		int top = rect.getTop() < invalid.getTop() ? rect.getTop() : invalid.getTop();
		int left = rect.getLeft() < invalid.getLeft() ? rect.getLeft() : invalid.getLeft();
		int bottom = rect.getBottom() > invalid.getBottom() ? rect.getBottom() : invalid.getBottom();
		int right = rect.getRight() > invalid.getRight() ? rect.getRight() : invalid.getRight();

		invalid = Rectangle(left, top, right - left, bottom - top);
	}

	// Fix invalid area
	if (invalid.x < 0)
	{
		invalid.width += invalid.x;
		invalid.x = 0;
	}

	if (invalid.y < 0)
	{
		invalid.height += invalid.y;
		invalid.y = 0;
	}

	if (invalid.x + invalid.width > getBounds().width) invalid.width = getBounds().width - invalid.x;
	if (invalid.y + invalid.height > getBounds().height) invalid.height = getBounds().height - invalid.y;

}
