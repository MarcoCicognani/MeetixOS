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

#include <iostream>
#include <string>
#include <Utils/ArgsParser.hh>
#include <vector>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    auto files_paths = std::vector<std::string>{};

    auto args_parser = Utils::ArgsParser{ "File Classifier Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_positional_argument(files_paths,
                                        "Files to inspect and classify",
                                        "FileToInspect",
                                        true);

    /* parse the arguments */
    args_parser.parse(argc, argv);

    for ( auto& file_path : files_paths ) {
        std::cout << file_path << ": ";

        /* open the file-stream */
        auto file_stream = std::ifstream{ file_path };
        if ( !file_stream.is_open() ) {
            std::cerr << "Error: file '" << file_path << "' does not exists\n";
            continue;
        }

        /* try to inspect the file */
        auto inspector = Inspector::inspector_for_file(file_stream);
        if ( !inspector ) {
            std::cerr << "Error: no known inspector for file '" << file_path << "'\n";
            continue;
        }

        /* print-out the inspection */
        std::cout << inspector->inspection() << std::flush;
    }
    return EXIT_SUCCESS;
}
