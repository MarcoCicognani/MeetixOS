/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "Directory.hh"

#include <Api/utils/local.hpp>
#include <iomanip>
#include <iostream>
#include <LibUtils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

void print_header() {
    std::cout << std::setw(WIDTH_TYPE) << std::setfill(' ') << "Type";
    std::cout << std::setw(WIDTH_PROT) << std::setfill(' ') << "Prot";
    std::cout << std::setw(WIDTH_NAME) << std::setfill(' ') << "Name";
    std::cout << std::setw(WIDTH_INODE) << std::setfill(' ') << "Inode";
    std::cout << std::setw(WIDTH_SIZE) << std::setfill(' ') << "Size";
    std::cout << '\n';
}

int main(int argc, const char** argv) {
    bool show_hidden{ false };
    bool show_header{ false };

    std::string              sort_order{};
    std::vector<std::string> dirs_to_list{};

    Utils::ArgsParser args_parser{ "Lists Directory Entries", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(show_hidden, "Show hidden entries", "all", 'a');
    args_parser.add_option(show_header, "Show The header", "header", 'd');
    args_parser.add_option(sort_order, "Entries sort order", "sort", 's', "SortOrder");
    args_parser.add_positional_argument(dirs_to_list, "Directories to list", "DirToList", false);

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* the dirs_to_list are not required */
    if ( dirs_to_list.empty() )
        dirs_to_list.emplace_back(".");

    /* read the current working directory */
    std::string work_dir{};
    {
        Local work_dir_buffer{ new char[PATH_MAX] };

        auto work_dir_status = s_get_working_directory(work_dir_buffer());
        if ( work_dir_status != GET_WORKING_DIRECTORY_SUCCESSFUL ) {
            std::cerr << "Failed to obtain current working directory\n";
            return EXIT_FAILURE;
        }

        /* set the working directory */
        work_dir = work_dir_buffer();
    }

    /* print the header if necessary */
    if ( show_header )
        print_header();

    /* list each directory requested */
    for ( auto const& dir : dirs_to_list ) {
        /* absolute the path to list */
        std::string dir_to_list{};
        if ( !dir.starts_with('/') ) {
            std::stringstream ss{};
            ss << work_dir << '/' << dir;
            dir_to_list = ss.str();
        } else
            dir_to_list = dir;

        /* open the current directory */
        Directory directory{ dir_to_list, sort_order };

        /* collect all his entries */
        auto collected = directory.collect_entries();
        if ( !collected )
            std::cerr << "Failed to open: '" << dir_to_list << "'\n";

        /* print all the entries */
        directory.for_each([show_hidden](const Directory::Entry& entry) {
            if ( entry.m_name.starts_with('.') && !show_hidden )
                return;

            std::cout << entry.as_string() << '\n';
        });
    }
    return EXIT_SUCCESS;
}
