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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

/**
 * show the usage of the command
 */
int usage(const char *cmdname)
{
    println("%s command:", cmdname);
    println("usage: %s [test]", cmdname);
    println("Avaible tests:");
    println("\t--heap/-d          [allocs differents size of memory on the heap to deallocate after 5 seconds, then allocate a big size of memory that the test doesn't frees]");
    println("\t--thread/-t        [creates many threads that increments a counter, test the multitasking]");
    println("\t--fs/-f            [open many files and reads it]");

    return 0;
}

/**
 * test the process heap allocating many
 * sized pointer and then freeing it, last big pointer
 * allocated is not been frees by the tester but by the kernel that dealloc the process
 */
int testHeap()
{
    uint64_t *pointers[500];

    // iterate 200 times
    for (int i = 0; i < 500; i++)
    {
        uint32_t size = (sizeof(uint64_t) * i * 50);
        println("Allocating %i bytes of heap", size);
        pointers[i] = malloc(size);
        sched_yield();
    }

    for (int i = 0; i < 500; i++)
    {
        println("Freeing %i bytes of memory", sizeof(uint64_t) * i * 50);
        free(pointers[i]);
        sched_yield();
    }

    // allocate a very big pointer and don't remove
    uint32_t leaksize = sizeof(uint64_t) * 10000000;
    println("allocating a leak of %i", leaksize);
    uint64_t *leak = malloc(leaksize);
    sleep(5000);
    println("Bye bye");

    return 0;
}

/**
 * creates various threads
 */
int testThread()
{
    return 0;
}

int testFilesystem()
{
    return 0;
}


/**
 * main for stress test
 */
int main(int argc, const char *argv[])
{
    // create options
    struct option longopts[] = {
        { "heap", no_argument, 0, 'd' },
        { "thread", no_argument, 0, 't' },
        { "fs", no_argument, 0, 'f' },
        { 0, 0, 0, 0 }
    };

    // parse args
	char opt;
	while ((opt = getoptlong(argc, argv, "h?", longopts, NULL)) != EOF)
	{
		switch (opt)
		{
            case 'd': return testHeap();
            case 't': return testThread();
            case 'f': return testFilesystem();
			case 'h': return usage(argv[0]);
			case '?': return usage(argv[0]);
		}
	}

    return 0;
}
