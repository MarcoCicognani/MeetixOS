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

#include <eva.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <tasking/tasking.hpp>

/*
 * show the usage of command
 */
void usage(const char *cmdname)
{
	println("");
	println("Thread displayer utility");
	println("usage: %s [filter]", cmdname);
	println("The following filters are available:");
	println("\t-i/--sort-by-tid           show the task list sorted by the Thread numeric id");
	println("\t-n/--sort-by-name          show the task list sorted by the Thread string id");
	println("\t-m/--sort-by-mem           show the task list sorted by the Thread memory use");
	println("\t-h/--help                  show this help");
	println("");
}

// compare by id
static inline bool compareByTid(const ThreadDescriptor &task1, const ThreadDescriptor &task2) { return task1.id > task2.id; }

// compare by Name
static inline bool compareByName(const ThreadDescriptor &task1, const ThreadDescriptor &task2) { return strcmp(task1.identifier, task2.identifier) > 0; }

// compare by Memory
static inline bool compareByMem(const ThreadDescriptor &task1, const ThreadDescriptor &task2) { return task1.threadUserStackSize > task2.threadUserStackSize; }

/*
 *
 */
int main(int argc, const char *argv[])
{
	// create help flag
	bool showHelp = false;

	// create args
	option longopts[] = {
		{ "sort-by-id", no_argument, 0, 'i' },
		{ "sort-by-name", no_argument, 0, 'n' },
		{ "sort-by-mem", no_argument, 0, 'm' },
		{ "help", no_argument, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	// set as default the comparator t
	static bool (*compare)(const ThreadDescriptor &task1, const ThreadDescriptor &task2) = &compareByTid;

	// parse args
	char opt;
	while ((opt = getoptlong(argc, argv, "h", longopts, NULL)) != EOF)
	{
		switch (opt)
		{
			case 'i': compare = &compareByTid; break;
			case 'n': compare = &compareByName; break;
			case 'm': compare = &compareByMem; break;
			case 'h': showHelp = true; break;
		}
	}

	// check if is not requested the help
	if (!showHelp)
	{
		// get thread list from kernel
		llist<ThreadDescriptor> tlist = Tasking::getThreads();
		tlist.sort(compare);

		// print on screen
		println("%5s %-56s %-20s %-20s   %5s", "Tid", "Name", "Execution time (ms)", "Scheduled Time", "User Stack (kb)");
		for (llist<ThreadDescriptor>::iterator it = tlist.begin(); it != tlist.end(); ++it)
			println("%5i %-56s %-20d %-20d %5ikb", (*it).id, (*it).identifier, (*it).executionTime, (*it).scheduledTimes, (*it).threadUserStackSize);
	}

	else usage(argv[0]);

	return 0;
}
