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

#ifndef __MEETIX_LIBC_MALLOC__
#define __MEETIX_LIBC_MALLOC__

#include "eva/common.h"
#include <stddef.h>

#define _GLIBCXX_HAVE_ALIGNED_ALLOC 1

__BEGIN_C

/**
 * Performs a memory allocation with a size of <size> that is aligned
 * to the <alignment>. (N1548-7.22.3.1)
 *
 * @param alignment:    alignment to use
 * @param size:         size to request
 * @return allocated space or NULL if not successful
 */
void *aligned_alloc(size_t alignment, size_t size);

/**
 * Allocates <num> objects of <size> bytes, all bits set to zero. (N1548-7.22.3.2)
 *
 * @param num:      number of objects
 * @param size:     size of each object
 * @return allocated space or NULL if not successful
 */
void *calloc(size_t num, size_t size);

/**
 * Deallocates the given memory (which must have previously been allocated
 * with one of the allocation functions). (N1548-7.22.3.3)
 *
 * @param ptr:      pointer to memory to free, or NULL
 */
void free(void *ptr);

/**
 * Allocates <size> bytes of memory. (N1548-7.22.3.4)
 *
 * @param size:     size in bytes to allocate
 * @return allocated space or NULL if not successful
 */
void *malloc(size_t size);

/**
 * Reallocates the memory pointed to by <ptr> to have at least
 * <size> bytes after the allocation. (N1548-7.22.3.5)
 *
 * @param ptr:      memory to reallocate
 * @param size:     size in bytes to allocate
 * @return allocated space or NULL if not successful
 */
void *realloc(void *ptr, size_t size);

__END_C

#endif
