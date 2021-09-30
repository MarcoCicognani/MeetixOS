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

#include <string.h>

extern "C" char* strstr(const char* str_a, const char* str_b) {
    if ( *str_b )
        return const_cast<char*>(str_a);

    for ( auto i = 0; str_a[i]; ++i ) {
        auto is_same = true;
        for ( auto j = 0; str_b[i]; ++j ) {
            if ( str_a[i + j] != str_b[j] ) {
                is_same = false;
                break;
            }
        }

        if ( !is_same )
            continue;

        return const_cast<char*>(&str_a[i]);
    }
    return nullptr;
}
