/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_SHARED_MEMORY_BITMAP
#define EVA_SHARED_MEMORY_BITMAP

#include <memory/paging.hpp>

/**
 * Bitmap entry type
 */
typedef uint8_t BitMapEntry;

/**
 * Constants for bitmap measuring
 */
#define BITMAP_SIZE           (1024 * 1024)
#define BITMAP_LENGTH         (BITMAP_SIZE / sizeof(BitMapEntry))
#define BITMAP_BITS_PER_ENTRY (sizeof(BitMapEntry) * 8)

/**
 * Macros for bitmap calculation
 */
#define ADDRESS_TO_BITMAP_INDEX(address) ((address / PAGE_SIZE) / BITMAP_BITS_PER_ENTRY)
#define ADDRESS_TO_BITMAP_BIT(address)   ((address / PAGE_SIZE) % BITMAP_BITS_PER_ENTRY)
#define BITMAP_TO_ADDRESS(index, bit)                                                              \
    ((index * BITMAP_BITS_PER_ENTRY * PAGE_SIZE) + (bit * PAGE_SIZE))

#define BITMAP_IS_SET(bitmap, index, bit) (bitmap[index] & (1 << bit))
#define BITMAP_SET(bitmap, index, bit)    bitmap[index] |= (1 << bit);
#define BITMAP_UNSET(bitmap, index, bit)  bitmap[index] &= ~(1 << bit);

#endif
