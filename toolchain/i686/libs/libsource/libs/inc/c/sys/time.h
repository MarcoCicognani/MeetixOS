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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef __MEETIX_LIBC_SYS_TIME__
#define __MEETIX_LIBC_SYS_TIME__

#include "eva/common.h"
#include <time.h>

__BEGIN_C

/**
 * timeval
 */
struct timeval
{
	time_t tv_sec;			// seconds
	suseconds_t tv_usec;	// micro seconds
};

/**
 * timezone
 */
struct timezone
{
    int tz_minuteswest;		// minutes west of Greenwich
    int tz_dsttime;			// type of DST correction
};

/**
 * Get the current time fillind the timeval and the timezone object
 *
 * @param tp:		timeval pointer
 * @param tzp:		timezone pointer
 * @return 0 on success -1 otherwise
 */
int gettimeofday(struct timeval *tp, struct timezone *tzp);

__END_C

#endif
