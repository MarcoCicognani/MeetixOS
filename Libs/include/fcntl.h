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

#ifndef __MEETIX_LIBC_FCNTL__
#define __MEETIX_LIBC_FCNTL__

#include "eva/fs.h"
#include "eva/stdint.h"

__BEGIN_C

// mode type
typedef uint32_t mode_t;

/**
 * POSIX open flags
 */
#define O_READ				FILE_FLAG_MODE_READ
#define O_WRITE				FILE_FLAG_MODE_WRITE
#define O_APPEND			FILE_FLAG_MODE_APPEND
#define O_CREAT 			FILE_FLAG_MODE_CREATE
#define O_TRUNC				FILE_FLAG_MODE_TRUNCATE
#define O_EXCL				FILE_FLAG_MODE_EXCLUSIVE
#define	O_RDONLY			FILE_FLAG_MODE_READ
#define O_WRONLY 			FILE_FLAG_MODE_WRITE
#define O_RDWR				(FILE_FLAG_MODE_READ | FILE_FLAG_MODE_WRITE)

/**
 * commands & flags for fcntl()
 */
#define	F_DUPFD				0//FILE_CONTROL_DUPLICATE_DESCRIPTOR
#define F_GETFD				0//FILE_CONTROL_GET_DESCRIPTOR_FLAGS
#define F_SETFD				0//FILE_CONTROL_SET_DESCRIPTOR_FLAGS
#define F_SETFL				0//FILE_CONTROL_SET_STATUS_FLAGS
#define F_GETLK				0//FILE_CONTROL_GET_REC_LOCK_INFO
#define F_SETLK				0//FILE_CONTROL_SET_REC_LOCK_INFO
#define F_SETLKW			0//FILE_CONTROL_SET_REC_LOCK_INFO_WAIT
#define F_GETOWN            0
#define F_SETOWN            0

#define FD_CLOEXEC			0//FILE_CONTROL_FLAG_CLOSE_ON_EXEC

/**
 * POSIX wrapper for open syscall
 *
 * @param pathname:     the path of the file that we want to open
 * @param flags:        one of the open flag codes
 * @param ...:          other parameters specificable, like mode
 * @return the file descriptor or FD_NONE
 */
int open(const char *pathname, int flags, ...);

__END_C

#endif
