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
#include <tasking/tasking.hpp>
#include <unistd.h>

/**
 * create the type comparator
 */
typedef bool (*Compare)(const ProcessDescriptor& task1, const ProcessDescriptor& task2);

/*
 * compare by id
 */
static inline bool compareByPid(const ProcessDescriptor& task1, const ProcessDescriptor& task2) {
    return task1.main.id > task2.main.id;
}

/*
 * compare by Name
 */
static inline bool compareByName(const ProcessDescriptor& task1, const ProcessDescriptor& task2) {
    return strcmp(task1.main.identifier, task2.main.identifier) > 0;
}

/*
 * compare by Memory
 */
static inline bool compareByMem(const ProcessDescriptor& task1, const ProcessDescriptor& task2) {
    return task1.heapUsed > task2.heapUsed;
}

/*
 * get memory usage from kernel and show a percentage bar on screen
 */
void showTotalMemoryUsage() {
    EvaSysInfo info;
    Sysinfo(&info);

    // TODO get width from shell
    int width   = (info.totRam / 1024) / 10;
    int used    = (info.totRam / 1024) - (info.freeRam / 1024);
    int percent = used / 100 * width;
    int cells   = (width * percent) / 100;

    printf("\n  %-4s", "Total Memory Usage [");

    for ( int index = 0; index < width; index++ ) {
        if ( index <= cells )
            putc('|', stdout);
        else
            putc(' ', stdout);
    }

    printf("] %d/%dMB\n\n", used, info.totRam / 1024);
}

/**
 * show the memory usage and get the process list from the kernel, then print it on the screen
 */
void showproc(Compare compare, bool useMegabytes) {
    // show memory usage
    showTotalMemoryUsage();

    // get task list from kernel
    llist<ProcessDescriptor> proclist = Tasking::getProcess();

    // sorting task array
    proclist.sort(compare);

    // print on screen
    println("%5s %6s %6s %-18s %-38s", "PID", "HEAP", "IMAGE", "NAME", "PATH");
    for ( llist<ProcessDescriptor>::iterator it = proclist.begin(); it != proclist.end(); it++ ) {
        ProcessDescriptor data = *it;
        // if (useMegabytes)
        //{
        data.imageSize = (data.imageSize > 1024 ? data.imageSize / 1024 : data.imageSize);
        data.heapUsed  = (data.heapUsed > 1024 ? data.heapUsed / 1024 : data.heapUsed);
        //}
        println("%5d %6d %6d %-18s %-38s",
                data.main.id,
                data.heapUsed,
                data.imageSize / 1024,
                data.main.identifier,
                data.sourcePath);
    }
}

/*
 * print on screen the help
 */
void usage(const char* cmdname) {
    println("");
    println("Process displayer utility");
    println("usage: %s [filter]", cmdname);
    println("avaible filters:");
    println("\t-i/--sort-by-id    [show the task list sorted by the process numeric id]");
    println("\t-n/--sort-by-name  [show the task list sorted by the process string id]");
    println("\t-m/--sort-by-mem   [show the task list sorted by the process memory use]");
    println("\t-u/--use-megabytes [show the heap and the image size using MB as unit]");
    println("\t-h/-?              [show this help]");
    println("");
}

/**
 *	task manager utility
 */
int main(int argc, const char* argv[]) {
    // create mode flags
    bool showhelp     = false;
    bool useMegabytes = true;

    // create args
    option longopts[] = { { "sort-by-id", no_argument, 0, 'i' },
                          { "sort-by-name", no_argument, 0, 'n' },
                          { "sort-by-mem", no_argument, 0, 'm' },
                          { "use-megabytes", no_argument, 0, 'u' },
                          { 0, 0, 0, 0 } };

    // set as default the comparator t
    static bool (*compare)(const ProcessDescriptor& task1, const ProcessDescriptor& task2)
        = &compareByPid;

    // parse args
    char opt;
    while ( (opt = getoptlong(argc, argv, "h?", longopts, NULL)) != EOF ) {
        switch ( opt ) {
            case 'i':
                compare = &compareByPid;
                break;
            case 'n':
                compare = &compareByName;
                break;
            case 'm':
                compare = &compareByMem;
                break;
            case 'u':
                useMegabytes = true;
            case 'h':
                showhelp = true;
                break;
            case '?':
                showhelp = true;
                break;
        }
    }

    // do a mode
    if ( !showhelp )
        showproc(compare, useMegabytes);
    else
        usage(argv[0]);

    return 0;
}
