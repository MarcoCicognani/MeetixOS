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

#include <Api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * mode for application
 */
enum ENameMode_t
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
void usage(const char* cmdname) {
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
enum ENameMode_t parseArgs(int argc, const char* argv[]) {
    // create long opts
    struct option long_opts[] = {
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
    while ( (opt = getopt_long(argc, argv, "h", long_opts, NULL)) != EOF ) {
        switch ( opt ) {
            case 'a':
                return ALL;
            case 'o':
                return USER;
            case 'u':
                return ENAME;
            case 'l':
                return LOADER;
            case 'k':
                return KERNEL;
            case 'i':
                return INFO;
            case 'h':
                return HELP;
        }
    }

    return ALL;
}

/*
 * Evangelion Name command utility
 */
int main(int argc, const char* argv[]) {
    // parse the arguments and get mode
    enum ENameMode_t mode = parseArgs(argc, argv);

    // perform mode
    switch ( mode ) {
        case ALL:
            {
                // call kernel to get ename info
                KernelName ename;
                s_kernel_name(&ename);

                // print infos
                println("");
                println("%s on %s %s [Copyright (C) 2017, MeetiX OS Project]",
                        getenv("USER"),
                        getenv("HOSTNAME"),
                        getenv("VERSION"));
                println(" Loader: %-15s v%d.%d.%c",
                        ename.m_loader_name,
                        ename.m_loader_major,
                        ename.m_loader_minor,
                        ename.m_loader_patch);
                println(" Kernel: %-15s v%d.%d.%d.%c",
                        ename.m_kernel_name,
                        ename.m_kernel_major,
                        ename.m_kernel_minor,
                        ename.m_kernel_sub,
                        ename.m_kernel_patch);

                // call kernel to get some system infos
                SystemInfo info;
                s_system_info(&info);

                // print infos
                println("");
                println(" RAM: used %iMB on %iMB",
                        (info.m_memory_total_amount - info.m_memory_free_amount) / 1024,
                        info.m_memory_total_amount / 1024);
                println(" CPU: %d avaible core, vendor: %s", info.m_cpu_count, info.m_cpu_vendor);
                println("");
            }
            break;

        case USER:
            {
                println("");
                println("%s on %s %s [Copyright (C) 2017, MeetiX OS Project]",
                        getenv("USER"),
                        getenv("HOSTNAME"),
                        getenv("VERSION"));
                println("");
            }
            break;

        case ENAME:
            {
                // call kernel to get ename info
                KernelName ename;
                s_kernel_name(&ename);

                // print infos
                println("");
                println(" Loader: %-15s v%d.%d.%c",
                        ename.m_loader_name,
                        ename.m_loader_major,
                        ename.m_loader_minor,
                        ename.m_loader_patch);
                println(" Kernel: %-15s v%d.%d.%d.%c",
                        ename.m_kernel_name,
                        ename.m_kernel_major,
                        ename.m_kernel_minor,
                        ename.m_kernel_sub,
                        ename.m_kernel_patch);
                println("");
            }
            break;

        case LOADER:
            {
                // call kernel to get ename info
                KernelName ename;
                s_kernel_name(&ename);

                // print infos
                println(" Loader: %-15s v%d.%d.%c",
                        ename.m_loader_name,
                        ename.m_loader_major,
                        ename.m_loader_minor,
                        ename.m_loader_patch);
            }
            break;

        case KERNEL:
            {
                // call kernel to get ename info
                KernelName ename;
                s_kernel_name(&ename);

                // print infos
                println(" Kernel: %-15s v%d.%d.%d.%c",
                        ename.m_kernel_name,
                        ename.m_kernel_major,
                        ename.m_kernel_minor,
                        ename.m_kernel_sub,
                        ename.m_kernel_patch);
            }
            break;

        case INFO:
            {
                // call kernel to get some system infos
                SystemInfo info;
                s_system_info(&info);

                // print infos
                println("");
                println(" RAM: used %iMB on %iMB",
                        (info.m_memory_total_amount - info.m_memory_free_amount) / 1024,
                        info.m_memory_total_amount / 1024);
                println(" CPU: %d avaible core, vendor: %s", info.m_cpu_count, info.m_cpu_vendor);
                println("");
            }
            break;

        case HELP:
            usage(argv[0]);
            break;
    }
}
