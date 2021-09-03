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

#ifndef MEETIX_LIBRARY_GRAPHICS_METRICS_RECTANGLE
#define MEETIX_LIBRARY_GRAPHICS_METRICS_RECTANGLE

#include <cstdint>
#include <graphics/metrics/point.hpp>

/**
 * container class to manage rectangle form
 */
struct Rectangle {
public:
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;

    /**
     * empty constructor
     */
    Rectangle() : x(0), y(0), width(0), height(0) {
    }

    /**
     * filled constructor
     *
     * @param x:		x value
     * @param y:		y value
     * @param width:	width value
     * @param height:	height value
     */
    Rectangle(int32_t x, int32_t y, int32_t width, int32_t height)
        : x(x), y(y), width(width), height(height) {
    }

    /**
     * copy constructor
     */
    Rectangle(const Rectangle& rhs) : x(rhs.x), y(rhs.y), width(rhs.width), height(rhs.height) {
    }

    /**
     * copy operator
     *
     * @param rhs:	the object to copy
     */
    Rectangle& operator=(const Rectangle& rhs) {
        x      = rhs.x;
        y      = rhs.y;
        width  = rhs.width;
        height = rhs.height;
        return *this;
    }

    /**
     * comparison operators
     */
    bool operator==(const Rectangle& rhs) const {
        return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
    }
    bool operator!=(const Rectangle& rhs) const {
        return !(*this == rhs);
    }

    /**
     * return true if p is contained into rectangle area
     */
    bool contains(const Point& p) const {
        return (p.x >= x) && (p.y >= y) && (p.x < x + width) && (p.y < y + height);
    }

    /**
     * manage Y
     */
    int getTop() const {
        return y;
    }
    void setTop(int _top) {
        y = _top;
    }

    /**
     * manage X
     */
    int getLeft() const {
        return x;
    }
    void setLeft(int _left) {
        x = _left;
    }

    /**
     * manage height
     */
    int getBottom() const {
        return y + height;
    }
    void setBottom(int32_t _bottom) {
        height = _bottom - y;
    }

    /**
     * manage width
     */
    int getRight() const {
        return x + width;
    }
    void setRight(int _right) {
        width = _right - x;
    }

    /**
     * set x and y from p
     */
    void setStart(const Point& p) {
        x = p.x;
        y = p.y;
    }

    /**
     * return a point with x and y coordinates
     */
    Point getStart() const {
        return Point(x, y);
    }

    /**
     * set the end of rectangle
     */
    void setEnd(const Point& p) {
        width  = p.x - x;
        height = p.y - y;
    }

    /**
     * return the end of rectangle
     */
    Point getEnd() const {
        return Point(x + width, y + height);
    }

    /**
     * normalize dimension
     */
    Rectangle asNormalized() {
        Rectangle norm = *this;

        if ( norm.width < 0 ) {
            norm.x     = norm.x + norm.width;
            norm.width = -norm.width;
        }

        if ( norm.height < 0 ) {
            norm.y      = norm.y + norm.height;
            norm.height = -norm.height;
        }
        return norm;
    }
} __attribute__((packed));

#endif
