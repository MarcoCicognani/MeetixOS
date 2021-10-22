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

#include <Api/utils/local.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <Utils/ArgsParser.hh>
#include <vector>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    auto show_as_hex = false;
    auto show_header = false;
    auto no_errors   = true;
    auto files_paths = std::vector<std::string>{};

    auto args_parser = Utils::ArgsParser{ "Read Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(show_as_hex, "Shows the content as hexadecimal", "hex", 'x');
    args_parser.add_option(show_header, "Shows An header before each file", "header", 'd');
    args_parser.add_option(no_errors, "Silent errors", "no-errors", 'n');
    args_parser.add_positional_argument(files_paths, "Files to read & print", "FileToRead");

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* obtain the current working directory */
    auto work_dir = std::string{};
    {
        auto work_dir_buffer = Local{ new char[PATH_MAX] };
        auto work_dir_status = s_get_working_directory(work_dir_buffer());
        if ( work_dir_status != GET_WORKING_DIRECTORY_SUCCESSFUL ) {
            if ( show_header && !no_errors ) {
                std::cerr << "Failed to obtain current working directory\n";
                return EXIT_FAILURE;
            }
        }

        work_dir = work_dir_buffer();
    }

    /* read all the files */
    for ( auto& file_path : files_paths ) {
        /* open the file-stream */
        auto file_stream = std::ifstream{ file_path };
        if ( !file_stream.is_open() ) {
            if ( !no_errors )
                std::cerr << file_path << ": File doesn't exists\n";
            continue;
        }

        if ( show_header ) {
            std::cout << "File: ";
            if ( !file_path.starts_with('/') )
                std::cout << work_dir << '/';
            std::cout << file_path << "\n----------------------------------------";
        }
        if ( show_as_hex )
            std::cout << std::hex;

        /* print out the file-content */
        std::cout << file_stream.rdbuf();
    }
    return EXIT_SUCCESS;
}
