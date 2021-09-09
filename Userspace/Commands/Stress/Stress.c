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
#include <stdio.h>
#include <unistd.h>

/**
 * show the usage of the command
 */
int usage(const char *cmdname)
{
    println("%s command:", cmdname);
    println("usage: %s [-v][test]", cmdname);
    println("Avaible tests:");
    println("\t--heap          [test the heap]");
    println("\t--stack         [test the memory stack with recursive function and big local array]");
    println("\t--thread        [test the native implementation of the MeetiX's threads]");
    println("\t--pthread       [test the implementation of the Posix thread on MeetiX]");
    println("\t--fs            [open many files and reads it]");

    return 0;
}

/**
 * main for stress test
 */
int main(int argc, const char *argv[])
{
    uint8_t verbose = true;

    // create options
    struct option longopts[] = {
        { "heap", no_argument, 0, 'd' },
        { "stack", no_argument, 0, 's' },
        { "thread", no_argument, 0, 't' },
        { "pthread", no_argument, 0, 'p' },
        { "fs", no_argument, 0, 'f' },
        { 0, 0, 0, 0 }
    };

    // parse args
	char opt;
	while ((opt = getoptlong(argc, argv, "vh?", longopts, NULL)) != EOF)
	{
		switch (opt)
		{
            case 'd': return testHeap(verbose);
            case 's': return testStack(verbose);
            case 't': return testThread(verbose);
            case 'p': return testPosixThread(verbose);
            case 'f': return testFilesystem(verbose);
            case 'v': verbose = true;
			case 'h': return usage(argv[0]);
			case '?': return usage(argv[0]);
		}
	}

    return 0;
}
