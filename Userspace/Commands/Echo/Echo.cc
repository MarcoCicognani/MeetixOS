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

#include <iostream>
#include <string>
#include <LibUtils/ArgsParser.hh>
#include <vector>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    std::vector<std::string> args_to_echo{};
    bool                     no_newline = false;

    Utils::ArgsParser args_parser{ "Echo Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(no_newline, "Avoid print newline at last", "no-newline", 'n');
    args_parser.add_positional_argument(args_to_echo, "Values to print on screen", "ValueToPrint", false);

    /* parse the arguments */
    args_parser.parse(argc, argv);

    if ( args_to_echo.empty() ) {
        char c = '\0';
        while ( std::cin.read(&c, 1) )
            std::cout << c;
    } else {
        for ( auto const& value : args_to_echo )
            std::cout << value << " ";

        if ( !no_newline )
            std::cout << '\n';
    }

    std::cout << std::flush;
    return EXIT_SUCCESS;
}
