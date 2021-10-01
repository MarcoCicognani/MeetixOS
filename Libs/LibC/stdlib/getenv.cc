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

#ifndef LIBC_BUILDING_LIBSTDCXX
#    include <fstream>
#    include <string.h>
#    include <string>

extern "C" char* getenv(const char* key) {
    std::ifstream env_stream{ "/MeetiX/Configs/Env/Global" };
    if ( !env_stream.is_open() )
        return nullptr;

    std::string line;
    while ( std::getline(env_stream, line) ) {
        auto separator = line.find('=');
        if ( separator == std::string::npos )
            continue;

        auto str_key   = line.substr(0, separator);
        auto str_value = line.substr(separator + 1);
        if ( str_key == key ) {
            auto buffer = new char[str_value.length() + 1];
            strncpy(buffer, str_value.c_str(), str_value.length());

            return buffer;
        }
    }
    return nullptr;
}
#else
extern "C" char* getenv(const char*) {
    return nullptr;
}
#endif