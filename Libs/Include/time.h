/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                  *
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

#ifndef __MEETIX_LIBC_TIME__
#define __MEETIX_LIBC_TIME__

#include <Api/Common.h>
#include <stdio.h>

__BEGIN_C

/**
 * Typedefs types for time variables
 */
typedef long time_t;
typedef long suseconds_t;

/**
 * Typedefs types for time measurement
 */
typedef void* timer_t;
typedef int   clockid_t;
typedef long  clock_t;

/**
 * Macros are always gradited
 */
#define CLOCKS_PER_SEC 1000000L
#define TIME_UTC       1

/**
 *
 */
struct tm {
    int         tm_sec;   // Seconds. [0-60] (1 leap second)
    int         tm_min;   // Minutes. [0-59]
    int         tm_hour;  // Hours.   [0-23]
    int         tm_mday;  // Day.     [1-31]
    int         tm_mon;   // Month.   [0-11]
    int         tm_year;  // Year [1970; ...]
    int         tm_wday;  // Day of week. [0-6], 0=Sunday
    int         tm_yday;  // Days in year. [0-365]
    int         tm_isdst; // Daylight saving [-1/0/1]
    long        __tm_gmtoff;
    const char* __tm_zone;
};

/**
 *
 */
clock_t clock();

/**
 *
 */
time_t time(time_t* t);

/**
 *
 */
double difftime(time_t t1, time_t t2);

/**
 *
 */
time_t mktime(struct tm*);

/**
 *
 */
size_t strftime(char*, size_t, const char*, const struct tm*);

/**
 *
 */
char* strptime(const char* buf, const char* format, struct tm* tm);

/**
 *
 */
struct tm* gmtime(const time_t*);

/**
 *
 */
struct tm* localtime(const time_t*);

/**
 *
 */
char* asctime(const struct tm*);

/**
 *
 */
char* ctime(const time_t*);

__END_C

#endif
