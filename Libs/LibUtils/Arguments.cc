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

#include <LibUtils/Arguments.hh>
#include <sstream>

namespace Utils::Arguments {

std::vector<std::string> non_filtered(int argc, const char** argv) {
    /* iterate and select all the arguments which doesn't start with '-' */
    std::vector<std::string> args;
    for ( auto pos = 0; pos < argc; pos++ ) {
        auto arg_ptr = argv[pos];
        if ( arg_ptr[0] != '-' ) {
            args.emplace_back(std::string{ arg_ptr });
        }
    }
    return args;
}

std::vector<std::string> filtered(int argc, const char** argv) {
    /* iterate and select all the arguments which doesn't start with '-' */
    std::vector<std::string> args;
    for ( auto pos = 0; pos < argc; pos++ ) {
        auto arg_ptr = argv[pos];
        if ( arg_ptr[0] == '-' ) {
            args.emplace_back(std::string{ arg_ptr });
        }
    }
    return args;
}

template<typename T>
void split(const std::string& str, char delim, T result) {
    std::stringstream ss{ str };
    std::string       item;
    while ( std::getline(ss, item, delim) )
        *(result++) = item;
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> splits;
    split(str, delim, std::back_inserter(splits));
    return splits;
}

} /* namespace Utils::Arguments */