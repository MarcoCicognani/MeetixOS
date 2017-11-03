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
#include <stdlib.h>
#include <unistd.h>
#include <eva.h>

/*
 * mode for application
 */
enum EnameMode_t
{
	ALL,
	USER,
	ENAME,
	LOADER,
	KERNEL,
	INFO,
	HELP,
};

/*
 * show the help
 */
void usage(const char *cmdname)
{
	println("");
	println("%s command utility", cmdname);
	println("usage: %s [filter]", cmdname);
	println("avaible filters:");
	println("\t--all/-       [show all ename infos]");
	println("\t--os/-o       [show usarname, hostname, version and copyrigth]");
	println("\t--ename/-u    [show the name and version of the kernel]");
	println("\t--loader/-l   [show name and version of loader]");
	println("\t--kernel/-k   [show name and version of kernel]");
	println("\t--info/-i     [show hardware specifications]");
	println("");
}

/*
 * return one of mode in EnameMode_t
 */
enum EnameMode_t parseArgs(int argc, const char *argv[])
{
	// create long opts
	struct option longopts[] = {
		{ "all", required_argument, 0, 'a' },
		{ "os", required_argument, 0, 'o' },
		{ "ename", required_argument, 0, 'u' },
		{ "loader", required_argument, 0, 'l' },
		{ "kernel", required_argument, 0, 'k' },
		{ "info", required_argument, 0, 'i' },
		{ 0, 0, 0, 0 },
	};

	// parsing provided args
	char opt;
	while ((opt = getoptlong(argc, argv, "h", longopts, NULL)) != EOF)
	{
		switch (opt)
		{
			case 'a': return ALL;
			case 'o': return USER;
			case 'u': return ENAME;
			case 'l': return LOADER;
			case 'k': return KERNEL;
			case 'i': return INFO;
			case 'h': return HELP;
		}
	}

	return ALL;
}

/*
 * Evangelion Name command utility
 */
int main(int argc, const char *argv[])
{
	// parse the arguments and get mode
	enum EnameMode_t mode = parseArgs(argc, argv);

	// perform mode
	switch (mode)
	{
		case ALL:
		{
			// call kernel to get ename info
			EvaName ename;
			Ename(&ename);

			// print infos
			println("");
			println("%s on %s %s [Copyright (C) 2017, MeetiX OS Project]", getenv("USER"), getenv("HOSTNAME"), getenv("VERSION"));
			println(" Loader: %-15s v%d.%d.%c", ename.loaderName, ename.LversionMJ,  ename.LversionMN, ename.LversionPT);
			println(" Kernel: %-15s v%d.%d.%d.%c", ename.kernelName, ename.versionMJ, ename.versionMN, ename.versionSB, ename.versionPT);

			// call kernel to get some system infos
			EvaSysInfo info;
			Sysinfo(&info);

			// print infos
			println("");
			println(" RAM: used %iMB on %iMB", (info.totRam - info.freeRam) / 1024, info.totRam / 1024);
			println(" CPU: %d avaible core, vendor: %s", info.numberOfCore, info.cpuVendor);
			println("");
		}
		break;

		case USER:
		{
			println("");
			println("%s on %s %s [Copyright (C) 2017, MeetiX OS Project]", getenv("USER"), getenv("HOSTNAME"), getenv("VERSION"));
			println("");
		}
		break;

		case ENAME:
		{
			// call kernel to get ename info
			EvaName ename;
			Ename(&ename);

			// print infos
			println("");
			println(" Loader: %-15s v%d.%d.%c", ename.loaderName, ename.LversionMJ,  ename.LversionMN, ename.LversionPT);
			println(" Kernel: %-15s v%d.%d.%d.%c", ename.kernelName, ename.versionMJ, ename.versionMN, ename.versionSB, ename.versionPT);
			println("");
		}
		break;

		case LOADER:
		{
			// call kernel to get ename info
			EvaName ename;
			Ename(&ename);

			// print infos
			println(" Loader: %-15s v%d.%d.%c", ename.loaderName, ename.LversionMJ,  ename.LversionMN, ename.LversionPT);
		}
		break;

		case KERNEL:
		{
			// call kernel to get ename info
			EvaName ename;
			Ename(&ename);

			// print infos
			println(" Kernel: %-15s v%d.%d.%d.%c", ename.kernelName, ename.versionMJ, ename.versionMN, ename.versionSB, ename.versionPT);
		}
		break;

		case INFO:
		{
			// call kernel to get some system infos
			EvaSysInfo info;
			Sysinfo(&info);

			// print infos
			println("");
			println(" RAM: used %iMB on %iMB", (info.totRam - info.freeRam) / 1024, info.totRam / 1024);
			println(" CPU: %d avaible core, vendor: %s", info.numberOfCore, info.cpuVendor);
			println("");
		}
		break;

		case HELP: usage(argv[0]); break;
	}
}
