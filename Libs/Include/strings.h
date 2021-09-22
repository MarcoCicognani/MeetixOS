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

#ifndef __MEETIX_LIBC_STRINGS__
#define __MEETIX_LIBC_STRINGS__

#include <Api/Common.h>
#include <stddef.h>

__BEGIN_C

/**
 * compare the two string ignoring the characters case
 *
 * @param s1:       the first string
 * @param s2:       the second string
 * @return 0 if strings are equals
 */
int strcasecmp(const char* s1, const char* s2);

/**
 * compare the two string for <n> bytes ignoring the characters case
 *
 * @param s1:       the first string
 * @param s2:       the second string
 * @param n:        the number of bytes to compare
 * @return 0 if strings are equals
 */
int strncasecmp(const char* s1, const char* s2, size_t n);

__END_C

#endif
