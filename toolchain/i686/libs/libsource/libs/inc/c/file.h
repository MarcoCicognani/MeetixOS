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

#ifndef __MEETIX_LIBC_FILE__
#define __MEETIX_LIBC_FILE__

#include "eva/common.h"
#include "eva/fs.h"
#include "sys/types.h"
#include "stdint.h"

__BEGIN_C

#define G_FILE_UNGET_PRESERVED_SPACE		4			// space preserved for ungetc-calls

#define G_FILE_FLAG_EOF						(1 << 26)	// end of file reached
#define G_FILE_FLAG_ERROR					(1 << 27)	// when a file error has occured
#define G_FILE_FLAG_BUFFER_SET				(1 << 28)	// whether the buffer was ever set
#define G_FILE_FLAG_BUFFER_DIRECTION_READ	(1 << 29)	// last access was read
#define G_FILE_FLAG_BUFFER_DIRECTION_WRITE	(1 << 30)	// last access was write
#define G_FILE_FLAG_BUFFER_OWNER_LIBRARY	(1 << 31)	// buffer is owned by the library (was created in setvbuf for example)

// define a type from struct declaration
typedef struct FILE FILE;

/**
 * Represents a stream. Used by stdio-related functions.
 * (N1548-7.21.1-2)
 */
struct FILE
{
	// file informations
	File_t file_descriptor;												// file descriptor provided by the kernel
	uint8_t lock;														// operation atomic lock

	// file content informations
	uint8_t *buffer;													// loaded buffer of file content
	size_t buffer_size;													// size of loaded buffer
	uint8_t buffer_mode;												// buffered active flag
	size_t buffered_bytes_write;										// buffered bytes count to write
	size_t buffered_bytes_read;											// buffered bytes count to read
	size_t buffered_bytes_read_offset;									// buffered offset

	// file operations
	ssize_t (*impl_read)(void *buf, size_t len, FILE *stream);			// read implementation
	ssize_t (*impl_write)(const void *buf, size_t len, FILE *stream);	// write implementation
	int (*impl_seek)(FILE*, off_t, int);								// seek implementation
	off_t (*impl_tell)(FILE*);											// tell implementation
	int (*impl_close)(FILE*);											// close implementation
	FILE *(*impl_reopen)(const char*, const char*, FILE*);				// reopen implementation
	int (*impl_fileno)(FILE*);											// fileno implementation
	int (*impl_eof)(FILE*);												// eof implementation
	int (*impl_error)(FILE*);											// error implementation
	void (*impl_clearerr)(FILE*);										// clear error implementation
	void (*impl_seterr)(FILE*);											// set error implementation

	uint32_t flags;														// file flags

	// linkage pointers
	FILE *prev;															// previous opened file
	FILE *next;															// next opened file
};

__END_C

#endif
