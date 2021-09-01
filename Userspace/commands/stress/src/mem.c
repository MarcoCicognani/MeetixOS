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

#include "tests.h"
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

/**
 * test the process heap allocating many
 * sized pointer and then freeing it, last big pointer
 * allocated is not been frees by the tester but by the kernel that dealloc the process
 */
int testHeap(uint8_t verbose)
{
    uint64_t *pointers[500];

    // iterate 500 times
    println("Allocate memory chuncks");
    for (int i = 0; i < 500; i++)
    {
        uint32_t size = (sizeof(uint64_t) * i * 50);
        if (verbose) println("Allocating %i bytes of heap", size);
        pointers[i] = malloc(size);
        sched_yield();
    }

    println("Freeing all memory allocated");
    for (int i = 0; i < 500; i++)
    {
        if (verbose) println("Freeing %i bytes of memory", sizeof(uint64_t) * i * 50);
        free(pointers[i]);
        sched_yield();
    }

    // allocate a very big pointer and don't remove
    uint32_t leaksize = sizeof(uint64_t) * 30000000;
    if (verbose) println("allocating a leak of %i", leaksize);
    uint64_t *leak = malloc(leaksize);
    sleep(3);
    println("Bye bye");

    return 0;
}

/**
 * test the stack calling recursive himself
 * for many times, then create a very big array
 */
int i = 200;
int testStack(uint8_t verbose)
{
    // create recursion
    if (verbose) println("Recursion number %d", i);
    if (--i > 0) return testStack(verbose);
    uint64_t chunck[30000000];
    return 0;
}
