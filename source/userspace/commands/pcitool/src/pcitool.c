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

#include <stdio.h>
#include <string.h>
#include "list.h"

#define MAJOR	0
#define MINOR	1

/**
 *
 */
int main(int argc, char** argv)
{
	if (argc > 1)
	{
		char *command = argv[1];

		if (!strcmp(command, "list")) return pciList();

		else if(strcmp(command, "--help") == 0)
		{
			println("pcitool, v%i.%i", MAJOR, MINOR);
			println("This program allows users to list data about PCI devices in the system.");
			println("");
			println("The following commands are available:");
			println("");
			println("\tlist\t\tlists basic information for all PCI devices");
			println("");
		}

		else fprintf(stderr, "unknown command: %s\n", command);
	}

	else
	{
		fprintf(stderr, "usage:\n\t%s <command>\n", argv[0]);
		fprintf(stderr, "Type \"%s --help\" for a list of commands.\n", argv[0]);
		fprintf(stderr, "\n");
	}
}
