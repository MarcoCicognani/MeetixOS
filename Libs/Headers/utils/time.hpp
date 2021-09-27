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

#ifndef MEETIX_LIBRARY_UTILS_TIME
#define MEETIX_LIBRARY_UTILS_TIME

#include <Api.h>
#include <stdint.h>

/**
 *
 */
#define TIME_DRIVER_IDENTIFIER "time"

/**
 *
 */
typedef int TimeDriverCommand;
#define GET_TIME   ((TimeDriverCommand)0)
#define GET_UPTIME ((TimeDriverCommand)1)

/**
 *
 */
struct TimeDriverHeader {
    TimeDriverCommand command;
} A_PACKED;

/**
 *
 */
struct TimeDriverCall {
    // explicit default constructor with reset of all values
    TimeDriverCall() : second(0), minute(0), hour(0), day(0), month(0), year(0) {
    }

    uint16_t second;
    uint16_t minute;
    uint16_t hour;
    uint16_t day;
    uint16_t month;
    uint16_t year;
} A_PACKED;

/**
 *
 */
struct TimeDriverUptime {
    // explicit default constructor with reset of all values
    TimeDriverUptime() : second(0) {
    }

    uint16_t second;
} A_PACKED;

/**
 * static call for control of MeetiX Time
 */
class MXtime {
public:
    static bool getTime(TimeDriverCall& call);
    static bool getUptime(TimeDriverUptime& call);
};

#endif
