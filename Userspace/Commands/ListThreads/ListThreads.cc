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

#include <iomanip>
#include <iostream>
#include <LibTasking/Tasking.hh>
#include <LibUtils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

#define WIDTH_TID         5
#define WIDTH_NAME        56
#define WIDTH_EXE_TIME    20
#define WIDTH_SCHED_COUNT 20
#define WIDTH_STACK       5

int main(int argc, const char** argv) {
    Utils::ArgsParser args_parser{ "List Executing Threads Utility", V_MAJOR, V_MINOR, V_PATCH };

    /* parse the options */
    args_parser.parse(argc, argv);

    /* print out the header */
    std::cout << std::setw(WIDTH_TID) << std::setfill(' ') << std::left << "Tid";
    std::cout << std::setw(WIDTH_NAME) << std::setfill(' ') << std::left << "Name";
    std::cout << std::setw(WIDTH_EXE_TIME) << std::setfill(' ') << std::left << "Exec Time (ms)";
    std::cout << std::setw(WIDTH_SCHED_COUNT) << std::setfill(' ') << std::left << "Sched Count";
    std::cout << std::setw(WIDTH_STACK) << std::setfill(' ') << std::left << "User Stack";
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

    /* list the threads */
    for ( auto const& thread : Tasking::list_threads() ) {
        std::cout << std::setw(WIDTH_TID) << std::setfill(' ') << thread.m_tid;
        std::cout << std::setw(WIDTH_NAME) << std::setfill(' ') << thread.m_identifier;
        std::cout << std::setw(WIDTH_EXE_TIME) << std::setfill(' ') << thread.m_execution_time;
        std::cout << std::setw(WIDTH_SCHED_COUNT) << std::setfill(' ') << thread.m_scheduled_times;
        std::cout << std::setw(WIDTH_STACK) << std::setfill(' ') << size_value(thread.m_thread_user_stack_size);
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
