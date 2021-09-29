/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#ifndef _CMOS_HPP_
#define _CMOS_HPP_

#include <sstream>
#include <stdint.h>
#include <Utils/Time.hh>
#include <Utils/Utils.hh>

using namespace std;

/**
 *
 */
struct TIME {
    // for sure set to 0 each variable
    TIME() : second(0), minute(0), hour(0), day(0), month(0), year(0), century(0), upSecond(0) {
    }

    // time variables
    size_t second;
    size_t minute;
    size_t hour;
    size_t day;
    size_t month;
    size_t year;
    size_t century;

    // uptime in second
    uint16_t upSecond;
};

/**
 *
 */
class Cmos_t {
public:
    /**
     *
     */
    Cmos_t() {
    }

    /**
     *  with register index read the rtc and fill the variable
     */
    void readRTC();

    /**
     * thread for timer, increase variables of global time
     */
    static void timer(Cmos_t* instance);

private:
    /**
     * ask to bios if there are updates
     */
    int getUpdateInProgressFlag();

    /**
     * return index of rtc registers
     */
    uint8_t getRTCregister(int reg);

    /**
     *
     */
    const uint32_t cmosAddress = 0x70;
    const uint32_t cmosData    = 0x71;
};

#endif
