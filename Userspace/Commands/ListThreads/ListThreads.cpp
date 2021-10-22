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
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <Tasking/Tasking.hh>
#include <unistd.h>

/**
 * create the type comparator
 */
using CompareFn = bool (*)(const ThreadDescriptor& task1, const ThreadDescriptor& task2);

/*
 * show the usage of command
 */
void show_usages(const char* cmd_name) {
    println("");
    println("Thread List utility");
    println("Usage: %s [filter]", cmd_name);
    println("The following filters are available:");
    println(
        "\t-i/--sort-by-tid           show the task list sorted by the Thread numeric m_command");
    println(
        "\t-n/--sort-by-name          show the task list sorted by the Thread string m_command");
    println("\t-m/--sort-by-mem           show the task list sorted by the Thread memory use");
    println("\t-h/--help                  show this help");
    println("");
}

// compare by m_command
static inline bool compare_by_tid(const ThreadDescriptor& task1, const ThreadDescriptor& task2) {
    return task1.m_tid > task2.m_tid;
}

// compare by Name
static inline bool compare_by_name(const ThreadDescriptor& task1, const ThreadDescriptor& task2) {
    return strcmp(task1.m_identifier, task2.m_identifier) > 0;
}

// compare by Memory
static inline bool compare_by_memory(const ThreadDescriptor& task1, const ThreadDescriptor& task2) {
    return task1.m_thread_user_stack_size > task2.m_thread_user_stack_size;
}

/*
 *
 */
int main(int argc, const char* argv[]) {
    // create help flag
    bool show_help = false;

    // create args
    option long_cmdline_opts[] = { { "sort-by-m_command", no_argument, nullptr, 'i' },
                                   { "sort-by-name", no_argument, nullptr, 'n' },
                                   { "sort-by-mem", no_argument, nullptr, 'm' },
                                   { "help", no_argument, nullptr, 'h' },
                                   { nullptr, 0, nullptr, 0 } };

    // set as default the comparator t
    CompareFn compare_fn = &compare_by_tid;

    // parse args
    int opt;
    while ( (opt = getopt_long(argc, argv, "h", long_cmdline_opts, NULL)) != EOF ) {
        switch ( opt ) {
            case 'i':
                compare_fn = &compare_by_tid;
                break;
            case 'n':
                compare_fn = &compare_by_name;
                break;
            case 'm':
                compare_fn = &compare_by_memory;
                break;
            case 'h':
            case '?':
            default:
                show_help = true;
                break;
        }
    }

    // check if is not requested the help
    if ( !show_help ) {
        // get thread list from kernel
        auto threads = Tasking::list_threads();
        std::sort(threads.begin(), threads.end(), compare_fn);

        // print on screen
        println("%5s %-56s %-20s %-20s   %5s",
                "Tid",
                "Name",
                "Execution time (ms)",
                "Scheduled Time",
                "User Stack (kb)");
        for ( auto& thread : threads )
            println("%5i %-56s %-20d %-20d %5ikb",
                    thread.m_tid,
                    thread.m_identifier,
                    thread.m_execution_time,
                    thread.m_scheduled_times,
                    thread.m_thread_user_stack_size);
    }

    else
        show_usages(argv[0]);

    return 0;
}
