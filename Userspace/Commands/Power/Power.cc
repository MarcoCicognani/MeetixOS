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

#include <cstring>
#include <iostream>
#include <Utils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    u32  sleep_time      = 0;
    bool reboot_system   = false;
    bool shutdown_system = false;

    Utils::ArgsParser args_parser{ "Power Management Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(sleep_time, "Sleep time", "sleep", 'l', "SleepTime");
    args_parser.add_option(reboot_system, "Reboot the computer", "reboot", 'r');
    args_parser.add_option(shutdown_system, "Shutdown the computer", "shutdown", 's');

    /* parse the options */
    args_parser.parse(argc, argv);

    /* the argument parser cannot check this */
    if ( !reboot_system && !shutdown_system ) {
        std::cerr << "Unknown operation to perform\n";
        return EXIT_FAILURE;
    }

    std::cout << "The system is going to " << (reboot_system ? "reboot" : "shutdown") << '\n';
    if ( sleep_time > 0 ) {
        std::cout << " in " << sleep_time << " seconds...\n";

        /* lets sleep */
        s_sleep(sleep_time * 1000);
    }

    /* execute the requested operation */
    if ( reboot_system )
        s_reboot_system();
    else
        s_shutdown_system();
    return EXIT_SUCCESS;
}
