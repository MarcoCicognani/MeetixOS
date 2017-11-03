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

#ifndef GRIDLAYOUTMANAGER_HPP_
#define GRIDLAYOUTMANAGER_HPP_

#include <layout/LayoutManager.hpp>
#include <graphics/metrics/insets.hpp>

/**
 *
 */
class GridLayoutManager_t: public LayoutManager_t
{
private:
	int columns;
	int rows;
	Insets padding;
	int horizontalCellSpace;
	int verticalCellSpace;

public:
	GridLayoutManager_t(int columns, int rows);
	void setPadding(Insets _padding)  { padding = _padding; }
	Insets getPadding() const  { return padding; }
	void setHorizontalCellSpace(int _space)  { horizontalCellSpace = _space; }
	int getHorizontalCellSpace() const  { return horizontalCellSpace; }
	void setVerticalCellSpace(int _space)  { verticalCellSpace = _space; }
	int getVerticalCellSpace() const  { return verticalCellSpace; }

	virtual void layout();
};

#endif
