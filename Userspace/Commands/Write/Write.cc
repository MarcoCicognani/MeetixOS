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
#include <Utils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    bool        append_content = false;
    std::string file_path{};

    Utils::ArgsParser args_parser{ "Write Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(append_content, "Appends the content to the destination file", "append", 'a');
    args_parser.add_positional_argument(file_path, "Destination file path", "FileToWrite");

    /* parse the arguments */
    args_parser.parse(argc, argv);

    auto stream_mode = append_content ? std::ios::app | std::ios::out : std::ios::out;

    /* open the output stream */
    std::ofstream out_stream{ file_path, stream_mode };
    if ( !out_stream.is_open() ) {
        std::cerr << "Unable to open/create '" << file_path << '\'' << std::endl;
        return EXIT_FAILURE;
    }

    /* read and fill buffer */
    std::string buffer{};
    while ( std::getline(std::cin, buffer) )
        out_stream << buffer;

    return EXIT_SUCCESS;
}
