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

#ifndef MEETIX_LIBRARY_GRAPHICS_METRICS_DIMENSION
#define MEETIX_LIBRARY_GRAPHICS_METRICS_DIMENSION

#include <cstdint>
#include "point.hpp"

/**
 * container class to manage dimension parameters
 */
class Dimension
{
public:
	int32_t width;
	int32_t height;

	/**
	 * empty constructor
	 */
	Dimension() : width(0), height(0) {}

	/**
	 * filled constructor
	 */
	Dimension(int32_t width, int32_t height) : width(width), height(height) {}

	/**
	 * copy constructor
	 */
	Dimension(const Dimension &p) : width(p.width), height(p.height) {}

	/**
	 * copy operator
	 */
	Dimension &operator=(const Dimension &rhs)
	{
		width = rhs.width;
		height = rhs.height;
		return *this;
	}

	/**
	 * comparison operators
	 */
	bool operator==(const Dimension &p) const { return width == p.width && height == p.height; }
	bool operator!=(const Dimension &p) const { return !(*this == p); }

	/**
	 * arithmetic operators
	 */
	Point operator-(const Dimension &p) const { return Point(width - p.width, height - p.height); }
	Point operator+(const Dimension &p) const { return Point(width + p.width, height + p.height); }

}__attribute__((packed));

#endif
