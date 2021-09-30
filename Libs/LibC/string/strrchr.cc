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

extern "C" char* strrchr(const char* str, int c) {
    auto rev_str = str + strlen(str);

    while ( true ) {
        if ( *rev_str == c )
            return const_cast<char*>(rev_str);
        if ( str == rev_str )
            break;

        --rev_str;
    }
    return nullptr;
}
