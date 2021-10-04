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

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#define VERSION "0.0.1"

int show_usages(int, char** argv) {
    std::cout << "Copy Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " Source Destination\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    /* show help/usages if request by the command-line */
    if ( getopt_is_help(argc, argv) || argc != 3 )
        return show_usages(argc, argv);

    auto src_path = std::string{ argv[1] };
    auto dst_path = std::string{ argv[2] };

    /* open the source file to copy */
    auto src_stream = std::ifstream{ src_path };
    if ( !src_stream.is_open() ) {
        std::cerr << "Failed to open '" << src_path << "'" << std::endl;
        return EXIT_FAILURE;
    }

    /* open or create the destination file */
    auto dst_stream = std::ofstream{ dst_path };
    if ( !dst_stream.is_open() ) {
        std::cerr << "Failed to open '" << dst_path << "'" << std::endl;
        return EXIT_FAILURE;
    }

    /* copy the file content */
    dst_stream << src_stream.rdbuf();
    return EXIT_SUCCESS;
}
