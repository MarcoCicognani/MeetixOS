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

#include "MemoryUsage.hh"

#include <iomanip>
#include <iostream>
#include <LibTasking/Tasking.hh>
#include <LibUtils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

#define WIDTH_PID   5
#define WIDTH_HEAP  6
#define WIDTH_IMAGE 6
#define WIDTH_NAME  18
#define WIDTH_PATH  40

int main(int argc, const char** argv) {
    bool hide_mem_usage_bar = false;

    Utils::ArgsParser args_parser{ "List Executing Processes Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(hide_mem_usage_bar, "Do not show the memory usage bar", "no-mem-usage", 'n');

    /* parse the options */
    args_parser.parse(argc, argv);

    /* show the memory usage bar if not hidden */
    if ( !hide_mem_usage_bar )
        std::cout << memory_usage_bar() << std::endl;

    /* print out the header */
    std::cout << std::setw(WIDTH_PID) << std::setfill(' ') << std::left << "Pid";
    std::cout << std::setw(WIDTH_HEAP) << std::setfill(' ') << std::left << "Heap";
    std::cout << std::setw(WIDTH_IMAGE) << std::setfill(' ') << std::left << "Image";
    std::cout << std::setw(WIDTH_NAME) << std::setfill(' ') << std::left << "Name";
    std::cout << std::setw(WIDTH_PATH) << std::setfill(' ') << std::left << "Path";
    std::cout << '\n';

    auto size_value = [](usize value) {
        char multiplier = 'K';
        if ( value >= 1024 * 1024 ) {
            value      = value / 1024 / 1024;
            multiplier = 'G';
        } else if ( value >= 1024 ) {
            value      = value / 1024;
            multiplier = 'M';
        }

        std::stringstream ss{};
        ss << value << multiplier;
        return ss.str();
    };

    /* list the processes */
    for ( auto const& proc : Tasking::list_processes() ) {
        std::cout << std::setw(WIDTH_PID) << std::setfill(' ') << proc.m_main_thread.m_tid;
        std::cout << std::setw(WIDTH_HEAP) << std::setfill(' ') << size_value(proc.m_heap_size);
        std::cout << std::setw(WIDTH_IMAGE) << std::setfill(' ') << size_value(proc.m_image_size);
        std::cout << std::setw(WIDTH_NAME) << std::setfill(' ') << proc.m_main_thread.m_identifier;
        std::cout << std::setw(WIDTH_PATH) << std::setfill(' ') << proc.m_source_path;
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
