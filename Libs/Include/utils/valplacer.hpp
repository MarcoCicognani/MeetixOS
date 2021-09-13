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

#ifndef MEETIX_LIBRARY_VALUE_PLACER
#define MEETIX_LIBRARY_VALUE_PLACER

#include <cstdint>
#include <string.h>

/**
 *
 */
class ValuePlacer {
private:
    uint8_t* buf;
    uint32_t pos;

public:
    ValuePlacer(uint8_t* buf) : buf(buf), pos(0) {
    }

    template<typename T>
    void put(T val) {
        *((T*)&buf[pos]) = val;
        pos += sizeof(T);
    }

    void put(uint8_t* in, uint32_t len) {
        memcpy(&buf[pos], in, len);
        pos += len;
    }

    template<typename T>
    T get() {
        T val = *((T*)&buf[pos]);
        pos += sizeof(T);
        return val;
    }

    void get(uint8_t* out, uint32_t len) {
        memcpy(out, &buf[pos], len);
        pos += len;
    }
};

#endif
