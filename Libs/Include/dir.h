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

#ifndef __MEETIX_LIBC_DIR__
#define __MEETIX_LIBC_DIR__

#include <dirent.h>
#include <Api/Common.h>
#include <Api/FileSystem.h>
#include <stdint.h>
#include <sys/types.h>

__BEGIN_C

// create type from struct declaration
typedef struct DIR DIR;

/**
 * Represents a directory stream. Used by dirent-related functions.
 */
struct DIR {
    struct dirent*       entbuf;
    FsDirectoryIterator* iter;
};

__END_C

#endif
