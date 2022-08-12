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

#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern "C" char* mktemp(char* templ) {
    if ( !templ )
        return templ;

    auto templ_len = strlen(templ);
    if ( templ_len < 6 ) {
        templ[0] = '\0';
        return templ;
    }

    for ( auto i = templ_len - 6; i < templ_len; ++i ) {
        if ( templ[i] != 'X' ) {
            errno    = EINVAL;
            templ[0] = '\0';
            return templ;
        }

        templ[i] = static_cast<char>('a' + rand() % ('z' - 'a'));
    }
    return templ;
}
