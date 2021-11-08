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

#include <IO/Shell.hh>
#include <iomanip>
#include <iostream>

#define MEMORY_USAGE_HEADER      "Memory Usage ["
#define MEMORY_USAGE_HEADER_LEN  sizeof(MEMORY_USAGE_HEADER)
#define MEMORY_USAGE_TRAILER_LEN 15

std::string memory_usage_bar() {
    /* obtain the system information */
    SystemInfo system_info{};
    s_system_info(&system_info);

    std::stringstream ss;
    ss << MEMORY_USAGE_HEADER;

    /* calculate the available shell width for the bar */
    auto shell_width = IO::Shell::instance().dimension().m_width - MEMORY_USAGE_HEADER_LEN
                     - MEMORY_USAGE_TRAILER_LEN;

    /* show the bar with the memory usage */
    auto used_mem         = system_info.m_memory_total_amount - system_info.m_memory_free_amount;
    auto used_mem_percent = used_mem / system_info.m_memory_total_amount * 100;
    auto used_cells       = shell_width * used_mem_percent / 100;
    for ( auto i = 0; i < shell_width; ++i ) {
        if ( i <= used_cells )
            ss << '|';
        else
            ss << ' ';
    }

    /* divide the sizes to the nearest multiplier */
    char  multiplier{ 'K' };
    usize total_mem{ system_info.m_memory_total_amount };
    if ( used_mem >= 1024 * 1024 ) {
        used_mem   = used_mem / 1024 / 1024;
        total_mem  = total_mem / 1024 / 1024;
        multiplier = 'G';
    } else if ( used_mem >= 1024 ) {
        used_mem   = used_mem / 1024;
        total_mem  = total_mem / 1024;
        multiplier = 'M';
    }

    /* show the memory usage bar */
    ss << "] ";
    ss << std::setw(3) << std::setfill(' ') << used_mem;
    ss << '/';
    ss << std::setw(3) << std::setfill(' ') << total_mem;
    ss << " " << multiplier << "iB\n";
    return ss.str();
}
