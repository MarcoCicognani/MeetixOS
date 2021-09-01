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

#ifndef EVA_KERNEL_BYTEWISE
#define EVA_KERNEL_BYTEWISE

#include <stdarg.h>
#include <stddef.h>
#include "eva/common.h"

__BEGIN_C

/**
 * Little endian conversion of 4-byte value <val>, writing
 * into array <arr>.
 */
#define BW_PUT_LE_4(arr, val)	    \
{			                        \
	arr[0] = (val >> 24) & 0xFF;	\
	arr[1] = (val >> 16) & 0xFF;	\
	arr[2] = (val >> 8) & 0xFF;		\
	arr[3] = val & 0xFF;			\
};

/**
 * Little endian conversion of 4-byte value in array <arr>,
 * written into the value <val>.
 */
#define BW_GET_LE_4(arr, val)       \
{                                   \
	val = 0;						\
	val |= arr[0] << 24;			\
	val |= arr[1] << 16;			\
	val |= arr[2] << 8;				\
	val |= arr[3];					\
};

__END_C

#endif
