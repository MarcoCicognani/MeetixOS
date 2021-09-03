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

#ifndef MEETIX_LIBRARY_GRAPHICS_METRICS_INSETS
#define MEETIX_LIBRARY_GRAPHICS_METRICS_INSETS

#include <cstdint>
#include <graphics/metrics/point.hpp>

/**
 * container class to manage graphics insets
 */
class Insets {
public:
    int32_t top;
    int32_t left;
    int32_t bottom;
    int32_t right;

    /**
     * empty constructor
     */
    Insets() : top(0), left(0), bottom(0), right(0) {
    }

    /**
     * copy constructor
     */
    Insets(const Insets& rhs) : top(rhs.top), left(rhs.left), bottom(rhs.bottom), right(rhs.right) {
    }

    /**
     * filled constructor
     */
    Insets(int32_t top, int32_t left, int32_t bottom, int32_t right)
        : top(top), left(left), bottom(bottom), right(right) {
    }

    /**
     * copy operator
     */
    Insets& operator=(const Insets& rhs) {
        top    = rhs.top;
        left   = rhs.left;
        bottom = rhs.bottom;
        right  = rhs.right;
        return *this;
    }

    /**
     * comparison operators
     */
    bool operator==(const Insets& rhs) const {
        return top == rhs.top && left == rhs.left && bottom == rhs.bottom && right == rhs.right;
    }
    bool operator!=(const Insets& rhs) const {
        return !(*this == rhs);
    }

} __attribute__((packed));

#endif
