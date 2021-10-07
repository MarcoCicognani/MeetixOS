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

#include "Inspector.hh"

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#define VERSION "0.0.1"

int show_usages(int, const char** argv) {
    std::cout << "File Classifier Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " [Options] FileNames...\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;

    return EXIT_SUCCESS;
}

int main(int argc, const char** argv) {
    if ( getopt_is_help(argc, argv) || argc < 2 )
        return show_usages(argc, argv);

    /* collect filenames */
    for ( auto arg = 1; arg < argc; ++arg ) {
        auto file_path = std::string{ argv[arg] };
        std::cout << file_path << ": ";

        /* open the file-stream */
        auto file_stream = std::ifstream{ file_path };
        if ( !file_stream.is_open() ) {
            std::cerr << "Error: file '" << file_path << "' does not exists\n";
            continue;
        }

        /* try to inspect the file */
        auto inspector = Inspector::inspect_file(file_stream);
        if ( !inspector ) {
            std::cerr << "Error: no known inspector for file '" << file_path << "'\n";
            continue;
        }

        /* print-out the inspection */
        std::cout << inspector->inspection() << std::flush;
    }

    return EXIT_SUCCESS;
}
