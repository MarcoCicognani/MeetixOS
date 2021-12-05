/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "Tests/Tests.hh"

#include <iostream>
#include <string>
#include <Utils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    bool        verbose             = false;
    bool        test_all            = false;
    bool        test_heap_memory    = false;
    bool        test_stack_memory   = false;
    bool        test_native_threads = false;
    bool        test_posix_threads  = false;
    bool        test_filesystem     = false;
    std::string test_name{};

    Utils::ArgsParser args_parser{ "Stress Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_positional_argument(test_name, "Test name to run", "TestName", false);

    args_parser.add_option(verbose, "All the tests have verbosity enabled", "verbose", 'v');
    args_parser.add_option(test_all, "Executes all the available stress tests", "all", 'a');
    args_parser.add_option(test_heap_memory, "Executes the heap stress tests", "heap", 'h');
    args_parser.add_option(test_stack_memory, "Executes the stack stress tests", "stack", 's');
    args_parser.add_option(test_native_threads,
                           "Executes native thread stress tests",
                           "native-threads",
                           'n');
    args_parser.add_option(test_posix_threads,
                           "Executes POSIX thread stress tests",
                           "posix-threads",
                           'p');
    args_parser.add_option(test_filesystem,
                           "Executes filesystem stress tests",
                           "posix-threads",
                           'p');

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* need at least one option set */
    if ( !test_all || !test_heap_memory || !test_stack_memory || !test_native_threads
         || !test_posix_threads || !test_filesystem ) {
        args_parser.print_usage(std::cout, argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
