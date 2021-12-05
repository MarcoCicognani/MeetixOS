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
    std::string start_dir{};
    std::string file_name{};

    Utils::ArgsParser args_parser{ "Find Utility", V_MAJOR, V_MINOR, V_PATCH };
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
        auto path_str = entry.path().string();

        if ( path_str.ends_with(file_name) )
            std::cout << entry.path() << std::endl;
    }

    return EXIT_FAILURE;
}
