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

#include <iostream>
#include <LibGUI/Application.hh>
#include <LibGUI/Notification.hh>
#include <LibUtils/ArgsParser.hh>
#include <string>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    std::string msg_title{};
    std::string msg_content{};

    Utils::ArgsParser args_parser{ "GUI Notificator Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_positional_argument(msg_title, "Title of the message", "Title", true);
    args_parser.add_positional_argument(msg_content, "Content of the message", "Message", true);

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* open the communication with the window-server */
    auto open_status = UI::open();
    if ( open_status != UI_OPEN_STATUS_SUCCESSFUL ) {
        std::cerr << "Failed to open communication with the WindowServer" << std::endl;
        return EXIT_FAILURE;
    }

    /* send the notification */
    Notification::send(msg_title, msg_content);

    /* close the communication with the window-server */
    auto close_status = UI::close();
    if ( close_status != UI_CLOSE_STATUS_SUCCESSFUL )
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
