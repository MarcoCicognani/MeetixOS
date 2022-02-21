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

#include <libgen.h>
#include <LibTC/Assertions.hh>
#include <string.h>

static char s_dot[]   = ".";
static char s_slash[] = "/";

extern "C" {

char* dirname(char* path) {
    if ( path == nullptr )
        return s_dot;

    int len = strlen(path);
    if ( len == 0 )
        return s_dot;

    while ( len > 1 && path[len - 1] == '/' ) {
        path[len - 1] = 0;
        len--;
    }

    char* last_slash = strrchr(path, '/');
    if ( last_slash == nullptr )
        return s_dot;

    if ( last_slash == path )
        return s_slash;

    *last_slash = 0;
    return path;
}

char* basename(char* path) {
    if ( path == nullptr )
        return s_dot;

    auto len = strlen(path);
    if ( len == 0 )
        return s_dot;

    while ( len > 1 && path[len - 1] == '/' ) {
        path[len - 1] = 0;
        len--;
    }

    auto last_slash = strrchr(path, '/');
    if ( last_slash == nullptr )
        return path;

    if ( len == 1 ) {
        VERIFY_EQUAL(last_slash, path);
        VERIFY_EQUAL(path[0], '/');
        return s_slash;
    }

    return last_slash + 1;
}
}