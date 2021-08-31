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

void usage(const char *cmdname)
{
	println("");
	println("%s command utility", cmdname);
	println("usage: %s [option]", cmdname);
	println("avaible options:");
	println("\t-s/--shutdown [shutdown the system]");
	println("\t-r/--reboot   [reboot the system]");
	println("");
}

/*
 *
 */
int main(int argc, char *argv[])
{
	if (argc < 2) usage(argv[0]);
	else if (!strcmp(argv[1], "-s") || !strcmp(argv[1], "--shutdown")) ShutdownSystem();
	else if (!strcmp(argv[1], "-r") || !strcmp(argv[1], "--reboot")) RebootSystem();
	else usage(argv[0]);

	return 0;
}
