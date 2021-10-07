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

#include <gui/notification.hpp>
#include <gui/ui.hpp>
#include <iostream>
#include <unistd.h>

#define VERSION "0.0.1"

int show_usages(int argc, const char** argv) {
    std::cout << "GUI Notificator Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " \"Title\" \"Message\"\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;

    return EXIT_SUCCESS;
}

int main(int argc, const char* argv[]) {
    if ( getopt_is_help(argc, argv) || argc != 3 )
        return show_usages(argc, argv);

    /* open the communication with the window-server */
    auto open_status = UI::open();
    if ( open_status != UI_OPEN_STATUS_SUCCESSFUL ) {
        std::cerr << "Failed to open communication with the WindowServer" << std::endl;
        return EXIT_FAILURE;
    }

    /* send the notification */
    Notification::send(argv[1], argv[2]);

    /* close the communication with the window-server */
    auto close_status = UI::close();
    if ( close_status != UI_CLOSE_STATUS_SUCCESSFUL )
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
