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

#ifndef MEETIX_LIBRARY_GRAPHICS_METRICS_POINT
#define MEETIX_LIBRARY_GRAPHICS_METRICS_POINT

#include <cstdint>

/**
 * container class to manage point on screen
 */
class Point {
public:
    int32_t x;
    int32_t y;

    /**
     * empty constructor
     */
    Point() : x(0), y(0) {
    }

    /**
     * filled constructor
     */
    Point(int32_t _x, int32_t _y) : x(_x), y(_y) {
    }

    /**
     * copy constructor
     */
    Point(const Point& p) : x(p.x), y(p.y) {
    }

    /**
     * copy operator
     */
    Point& operator=(const Point& rhs) {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    /**
     * comparison operators
     */
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }
    bool operator!=(const Point& p) const {
        return !(*this == p);
    }

    /**
     * managing operators
     */
    Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y);
    }
    Point operator+(const Point& p) const {
        return Point(x + p.x, y + p.y);
    }

} __attribute__((packed));

#endif
