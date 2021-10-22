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
#include <string>
#include <Utils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    auto start_dir = std::string{};
    auto file_name = std::string{};

    auto args_parser = Utils::ArgsParser{ "Find Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_positional_argument(start_dir,
                                        "Initial directory where start search",
                                        "SourceDir",
                                        true);
    args_parser.add_positional_argument(file_name,
                                        "Filename to search in each sub-directory",
                                        "FileName",
                                        true);

    /* parse the arguments */
    args_parser.parse(argc, argv);

    for ( auto& entry : std::filesystem::recursive_directory_iterator(start_dir) ) {
        if ( entry.path().string().ends_with(file_name) ) {
            std::cout << "Found: " << entry.path() << std::endl;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}
