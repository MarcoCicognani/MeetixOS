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
#include <IO/Shell.hh>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <Utils/ArgsParser.hh>
#include <vector>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

std::optional<std::string> get_working_directory(bool show_header, bool no_errors) {
    Local work_dir_buffer{ new char[PATH_MAX] };

    auto work_dir_status = s_get_working_directory(work_dir_buffer());
    if ( work_dir_status != GET_WORKING_DIRECTORY_SUCCESSFUL ) {
        if ( show_header && !no_errors ) {
            std::cerr << "Failed to obtain current working directory\n";
            return {};
        }
    } else
        return { std::string{ work_dir_buffer() } };
}

int main(int argc, const char** argv) {
    bool show_as_hex   = false;
    bool show_header   = false;
    bool no_errors     = true;
    bool show_line_num = false;

    std::vector<std::string> files_paths{};

    Utils::ArgsParser args_parser{ "Read Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(show_as_hex, "Shows the content as hexadecimal", "hex", 'x');
    args_parser.add_option(show_header, "Shows An header before each file", "header", 'd');
    args_parser.add_option(no_errors, "Silent errors", "no-errors", 'n');
    args_parser.add_option(show_line_num, "In non-hex mode shows the line number", "line-number", 'l');
    args_parser.add_positional_argument(files_paths, "Files to read & print", "FileToRead");

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* obtain the current working directory */
    auto work_dir = get_working_directory(show_header, no_errors);
    if ( !work_dir.has_value() )
        return EXIT_FAILURE;

    /* read all the files */
    for ( auto const& file_path : files_paths ) {
        /* open the file-stream */
        std::ifstream file_stream{ file_path };
        if ( !file_stream.is_open() ) {
            if ( !no_errors )
                std::cerr << file_path << ": File doesn't exists\n";
            continue;
        }

        if ( show_header ) {
            std::cout << "File: ";
            if ( !file_path.starts_with('/') )
                std::cout << work_dir.value() << '/';
            std::cout << file_path << "\n----------------------------------------";
        }

        if ( show_as_hex ) {
            /* store the stream content into a string-stream */
            std::stringstream ss{};
            ss << file_stream.rdbuf();

            auto shell_width = IO::Shell::instance().dimension().m_width;

            int line_counter = 0;
            for ( auto c : ss.str() ) {
                std::cout << std::setfill('0') << std::setw(2) << std::hex << int{ c };

                /* put the space or the newline */
                if ( ++line_counter > shell_width / 6 ) {
                    std::cout << '\n';
                    line_counter = 0;
                } else
                    std::cout << ' ';
            }
        } else {
            std::string line{};
            usize       line_num = 0;
            while ( std::getline(file_stream, line) ) {
                if ( show_line_num )
                    std::cout << std::setfill(' ') << std::setw(3) << line_num++ << ' ';

                std::cout << line << '\n';
            }
        }

        /* flush the buffer of cout */
        std::cout << std::flush;
    }
    return EXIT_SUCCESS;
}
