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

#ifndef MEETIX_LIBRARY_UTILS_TIMESPAN
#define MEETIX_LIBRARY_UTILS_TIMESPAN

#include <eva/types.h>

/**
 * time span container, permits to create a formatted timing
 */
class TimeSpan
{
public:
	/**
	 * milliseconds conversions for measurement
	 */
	const uint64_t SECOND = 1000;
	const uint64_t MINUTE = 60000;
	const uint64_t HOUR = 3600000;
	const uint64_t DAY = 216000000;

	/**
	 * internal data
	 */
	uint64_t day;
	uint64_t hour;
	uint64_t minute;
	uint64_t second;
	uint64_t millisecond;

	/**
	 * empty contructor
	 */
	TimeSpan() : day(0), hour(0), minute(0), second(0), millisecond(0) {}

	/**
	 * filled constructor
	 *
	 * @param d:	day value
	 * @param h:	hour value
	 * @param m:	minute value
	 * @param s:	second value
	 * @param ms:	milliseconds value
	 */
	TimeSpan(uint64_t d, uint64_t h, uint64_t m, uint64_t s, uint64_t ms) : day(d), hour(h), minute(m), second(s), millisecond(ms) {}

	/**
	 * operators
	 */
	bool operator==(const TimeSpan &tm) const { return day == tm.day && hour == tm.day && minute == tm.minute && second == tm.second && millisecond == tm.millisecond; }
	bool operator!=(const TimeSpan &tm) const { return day != tm.day || hour != tm.day || minute != tm.minute || second != tm.second || millisecond != tm.millisecond; }

	/**
	 * @return the total time in milliseconds
	 */
	uint64_t getTotTime() const { return (millisecond + (second * SECOND) + (minute * MINUTE) + (hour * HOUR) + (day * DAY)); }

	/**
	 * reset the internal values
	 */
	void reset()
	{
		day = 0;
		hour = 0;
		minute = 0;
		second = 0;
		millisecond = 0;
	}
}__attribute__((packed));

#endif
