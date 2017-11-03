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

#ifndef __MEETIX_LIBC_LIBGEN__
#define __MEETIX_LIBC_LIBGEN__

#include "eva/common.h"

__BEGIN_C

/**
 * return the base name from the provided path
 *
 * @param path:     the path to check
 * @return the basename of the path on success
 */
char *basename(char *path);

/**
 * return the fir name from the provided path
 *
 * @param path:     the path to check
 * @return the basename of the path on success
 */
char *dirname(char *path);

__END_C

#endif
