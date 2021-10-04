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

#include <filesystem>
#include <iostream>
#include <unistd.h>

#define VERSION "0.0.1"

int show_usages(int argc, const char** argv) {
    std::cout << "Find Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " SourceDir FileName\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;

    return EXIT_SUCCESS;
}

int main(int argc, const char** argv) {
    for ( auto i = 0; i < argc; ++i )
        std::cout << argv[i] << std::endl;

    if ( getopt_is_help(argc, argv) || argc != 3 )
        return show_usages(argc, argv);

    for ( auto& entry : std::filesystem::recursive_directory_iterator(argv[1]) ) {
        if ( entry.path().string().ends_with(argv[2]) ) {
            std::cout << "Found: " << entry.path() << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_FAILURE;
}
