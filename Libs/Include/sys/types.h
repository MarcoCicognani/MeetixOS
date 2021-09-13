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

#ifndef __MEETIX_LIBC_TYPES__
#define __MEETIX_LIBC_TYPES__

#include <eva.h>
#include <eva/common.h>

__BEGIN_C

typedef Pid     pid_t;
typedef int64_t off_t;
typedef int64_t ssize_t;

typedef FsVirtID ino_t; // a file is identified by its virtual id
typedef FsVirtID dev_t; // a device is identified by the virtual id of the mountpoint

// TODO
typedef int nlink_t;
typedef int uid_t;
typedef int gid_t;
typedef int blksize_t;
typedef int blkcnt_t;

__END_C

#endif
