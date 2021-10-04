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
#include <unistd.h>

#define VERSION "0.0.1"

int show_usages(int, const char** argv) {
    std::cout << "Echo Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " [Arguments-To-Print]\n";
    std::cout << "\t -n:               Does not print newline at the end\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;

    return EXIT_SUCCESS;
}

int main(int argc, const char** argv) {
    if ( getopt_is_help(argc, argv) )
        return show_usages(argc, argv);

    bool print_newline = true;
    switch ( getopt(argc, argv, "n") ) {
        case 'n':
            print_newline = false;
            break;
        default:
            return show_usages(argc, argv);
    }

    if ( argc == 1 ) {
        char c;
        while ( std::cin.read(&c, 1) )
            std::cout << c;
    } else {
        for ( auto i = optind; i < argc; ++i )
            std::cout << argv[i] << " ";

        if ( print_newline )
            std::cout << '\n';
    }

    return EXIT_SUCCESS;
}
