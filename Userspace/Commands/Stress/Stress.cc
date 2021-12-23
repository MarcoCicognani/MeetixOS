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

#include "Tests/Manager.hh"

#include <iostream>
#include <string>
#include <Utils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    bool                     list_all_tests   = false;
    bool                     enable_verbosity = false;
    std::vector<std::string> test_names{};

    Utils::ArgsParser args_parser{ "Stress Tests Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(list_all_tests, "Lists all the available stress tests", "names", 'n');
    args_parser.add_option(enable_verbosity, "Enable extra verbosity for each test", "verbose", 'e');
    args_parser.add_positional_argument(test_names, "Tests to run", "TestName", false);

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* register all the tests */
    Tests::Manager::inst().register_all();

    /* need at least one option set */
    if ( list_all_tests )
        return Tests::Manager::inst().list_to_stdout();
    else if ( !test_names.empty() )
        return Tests::Manager::inst().run_tests(test_names, enable_verbosity);
    else {
        std::cerr << "No tests selected" << std::endl;
        return EXIT_FAILURE;
    }
}
