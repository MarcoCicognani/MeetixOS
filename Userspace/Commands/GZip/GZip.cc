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
#include <zlib.h>

#define VERSION "0.0.1"

int show_usages(int, const char** argv) {
    std::cout << "GNU Zip Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " FilesToExtract...\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;

    return EXIT_SUCCESS;
}

int main(int argc, const char** argv) {
    if ( getopt_is_help(argc, argv) || argc < 2 )
        return show_usages(argc, argv);

    for ( auto arg = 1; arg < argc; ++arg ) {
        /* check for right extension in filename */
        auto gz_path = std::string{ argv[arg] };
        if ( !gz_path.ends_with(".gz") ) {
            std::cerr << gz_path << ": Not sure this file is gzipped\n";
            continue;
        }

        /* open the gz-file */
        gzFile gz_file = gzopen(gz_path.c_str(), "r");
        if ( !gz_file ) {
            std::cerr << gz_path << ": Unable to open\n";
            continue;
        }

        /* open the output stream */
        auto dest_path   = gz_path.substr(0, gz_path.length() - 3);
        auto dest_stream = std::ofstream{ dest_path };
        if ( !dest_stream.is_open() ) {
            std::cerr << dest_path << ": Unable to open/create\n";
            gzclose(gz_file);
            continue;
        }

        /* extract from the archive */
        while ( !gzeof(gz_file) ) {
            /* read the extracted content */
            char buffer[4096];
            auto read_bytes = gzread(gz_file, &buffer, 4096);

            /* write-out to the file */
            dest_stream.write(reinterpret_cast<const char*>(&buffer), read_bytes);
        }

        gzclose(gz_file);
    }

    return EXIT_SUCCESS;
}
