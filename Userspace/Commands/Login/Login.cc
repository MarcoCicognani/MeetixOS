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
#include <IO/Shell.hh>
#include <iomanip>
#include <iostream>
#include <Utils/ArgsParser.hh>
#include <Utils/Environment.hh>
#include <Utils/PropertyFileParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

void prompt_and_read_username_and_password(std::string& username, std::string& password) {
    std::cout << "Username: ";
    std::cin >> username;

    std::cout << "Password: ";
    IO::Shell::instance().set_echo(false);
    std::cin >> password;
    IO::Shell::instance().set_echo(true);
    std::cout << '\n';
}

bool check_login_credentials(const std::string& username, const std::string& password) {
    /* open the users file */
    std::ifstream users_stream{ "/MeetiX/Configs/Credentials/Users" };
    if ( !users_stream.is_open() )
        return false;

    /* parse the file */
    Utils::PropertyFileParser file_parser{ users_stream };
    auto                      users_map = file_parser.properties();

    /* find the pair */
    return std::any_of(users_map.begin(), users_map.end(), [&](const auto& item) {
        return username == item.first && password == item.second;
    });
}

int main(int argc, const char** argv) {
    bool hide_welcome{ false };
    bool one_shoot_auth{ false };

    Utils::ArgsParser args_parser{ "Login Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(hide_welcome, "Hides the welcome message", "hide-welcome", 'w');
    args_parser.add_option(one_shoot_auth, "Asks for authentication one time", "one-shot", 's');

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* print the welcome message if not hidden */
    if ( !hide_welcome ) {
        std::cout << "\033[32m";
        std::cout << "Welcome in\n";
        std::cout << R"(      __  __                 _     _  __   __     ____     _____ )" << '\n';
        std::cout << R"(     |  \/  |               | |   (_) \ \ / /    / __ \   / ____|)" << '\n';
        std::cout << R"(     | \  / |   ___    ___  | |_   _   \ V /    | |  | | | (___  )" << '\n';
        std::cout << R"(     | |\/| |  / _ \  / _ \ | __| | |   > <     | |  | |  \___ \ )" << '\n';
        std::cout << R"(     | |  | | |  __/ |  __/ | |_  | |  / . \    | |__| |  ____) |)" << '\n';
        std::cout << R"(     |_|  |_|  \___|  \___|  \__| |_| /_/ \_\    \____/  |_____/ )";
        std::cout << "v" << Utils::Environment::version() << '\n';
        std::cout << '\n';

        std::cout << "\033[35mAn hobby operating system under heavy development\n";
        std::cout << "by \033[0mMarco Cicognani \033[32mmarco.cicognani@meetixos.org\033[0m\n\n";
    }

    std::string username{};
    while ( true ) {
        /* prompt and read username & password */
        std::string password{};
        prompt_and_read_username_and_password(username, password);

        /* check credentials */
        auto can_login = check_login_credentials(username, password);
        if ( can_login ) {
            std::stringstream ss{};
            ss << static_cast<char>(std::toupper(username[0])) << username.substr(1);

            /* set the username to the environment */
            Utils::Environment::set("USER", ss.str());
            std::cout << "\nWelcome in " << ss.str() << "!\n" << std::endl;

            return EXIT_SUCCESS;
        } else {
            std::cout << "\033[31mIncorrect username or password!\033[0m\n";
            if ( one_shoot_auth )
                return EXIT_FAILURE;
            else
                s_sleep(2000);
        }
    }
}