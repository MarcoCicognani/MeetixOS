/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#ifndef STRESS_TESTS
#define STRESS_TESTS

#include <stdint.h>

/**
 * test the process heap allocating many
 * sized pointer and then freeing it, last big pointer
 * allocated is not been frees by the tester but by the kernel that dealloc the process
 */
int testHeap(uint8_t verbose);

/**
 * test the stack calling recursive himself
 * for many times, then create a very big array
 */
int testStack(uint8_t verbose);

/**
 * test the native implementation of the threads on the system
 */
int testThread(uint8_t verbose);

/**
 * test the implementation of the Posix thread on the system
 */
int testPosixThread(uint8_t verbose);

/**
 * test the filesystem
 */
int testFilesystem(uint8_t verbose);

#endif
