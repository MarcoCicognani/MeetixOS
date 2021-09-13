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

#ifndef MEETIX_LIBRARY_GRAPHICS_METRICS_RANGE
#define MEETIX_LIBRARY_GRAPHICS_METRICS_RANGE

#include <cstdint>

/**
 * container class to manage ranges
 */
class Range {
private:
    int32_t first;
    int32_t last;

public:
    /**
     * empty constructor
     */
    Range() : first(0), last(0) {
    }

    /**
     * filled constructor
     */
    Range(int32_t _a, int32_t _b) {
        set(_a, _b);
    }

    /**
     * copy constructor
     */
    Range(const Range& p) : first(p.first), last(p.last) {
    }

    /**
     * set the range
     */
    void set(int32_t a, int32_t b) {
        first = a < b ? a : b;
        last  = a > b ? a : b;
    }

    /**
     * return the begin of range
     */
    int32_t getFirst() {
        return first;
    }

    /**
     * return the end of range
     */
    int32_t getLast() {
        return last;
    }

    /**
     * return the range length
     */
    int32_t getLength() {
        return last - first;
    }

    /**
     * copy operator
     */
    Range& operator=(const Range& rhs) {
        first = rhs.first;
        last  = rhs.last;
        return *this;
    }

    /**
     * comparison operators
     */
    bool operator==(const Range& p) const {
        return (first == p.first) && (last == p.last);
    }
    bool operator!=(const Range& p) const {
        return !(*this == p);
    }

} __attribute__((packed));

#endif
