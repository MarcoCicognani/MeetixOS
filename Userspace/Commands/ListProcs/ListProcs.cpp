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
using CompareFn = bool (*)(const ProcessDescriptor& task1, const ProcessDescriptor& task2);

/*
 * compare by id
 */
static inline bool compare_by_pid(const ProcessDescriptor& task1, const ProcessDescriptor& task2) {
    return task1.main.id > task2.main.id;
}

/*
 * compare by Name
 */
static inline bool compare_by_name(const ProcessDescriptor& task1, const ProcessDescriptor& task2) {
    return strcmp(task1.main.identifier, task2.main.identifier) > 0;
}

/*
 * compare by Memory
 */
static inline bool compare_by_memory(const ProcessDescriptor& task1,
                                     const ProcessDescriptor& task2) {
    return task1.heapUsed > task2.heapUsed;
}

/*
 * get memory usage from kernel and show a percentage bar on screen
 */
void showTotalMemoryUsage() {
    EvaSysInfo info;
    Sysinfo(&info);

    // TODO get width from shell
    auto width   = (info.totRam / 1024) / 10;
    auto used    = (info.totRam / 1024) - (info.freeRam / 1024);
    auto percent = used / 100 * width;
    auto cells   = (width * percent) / 100;

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
void show_processes(CompareFn compare, bool useMegabytes) {
    // show memory usage
    showTotalMemoryUsage();

    // get task list from kernel
    auto procs_list = Tasking::getProcess();
    std::sort(procs_list.begin(), procs_list.end(), compare);

    // print on screen
    println("%5s %6s %6s %-18s %-38s", "PID", "HEAP", "IMAGE", "NAME", "PATH");
    for ( auto& proc_data : procs_list ) {
        proc_data.imageSize
            = (proc_data.imageSize > 1024 ? proc_data.imageSize / 1024 : proc_data.imageSize);
        proc_data.heapUsed
            = (proc_data.heapUsed > 1024 ? proc_data.heapUsed / 1024 : proc_data.heapUsed);
        println("%5d %6d %6d %-18s %-38s",
                proc_data.main.id,
                proc_data.heapUsed,
                proc_data.imageSize / 1024,
                proc_data.main.identifier,
                proc_data.sourcePath);
    }
}

/*
 * print on screen the help
 */
void usage(const char* cmd_name) {
    println("");
    println("Process List utility");
    println("Usage: %s [filter]", cmd_name);
    println("Available filters:");
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
    bool show_help    = false;
    bool useMegabytes = true;

    // create args
    option long_cmdline_opts[] = { { "sort-by-id", no_argument, nullptr, 'i' },
                                   { "sort-by-name", no_argument, nullptr, 'n' },
                                   { "sort-by-mem", no_argument, nullptr, 'm' },
                                   { "use-megabytes", no_argument, nullptr, 'u' },
                                   { nullptr, 0, nullptr, 0 } };

    // set as default the comparator t
    CompareFn compare_fn = &compare_by_pid;

    // parse args
    int opt;
    while ( (opt = getoptlong(argc, argv, "h?", long_cmdline_opts, NULL)) != EOF ) {
        switch ( opt ) {
            case 'i':
                compare_fn = &compare_by_pid;
                break;
            case 'n':
                compare_fn = &compare_by_name;
                break;
            case 'm':
                compare_fn = &compare_by_memory;
                break;
            case 'u':
                useMegabytes = true;
            case 'h':
            case '?':
            default:
                show_help = true;
                break;
        }
    }

    // do a mode
    if ( !show_help ) {
        show_processes(compare_fn, useMegabytes);
    } else {
        usage(argv[0]);
    }

    return 0;
}
