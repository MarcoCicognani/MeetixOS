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

#include <libgen.h>
#include <string.h>

static char* simple_buffer(char c) {
    static char basename_buffer[2];

    basename_buffer[0] = c;
    basename_buffer[1] = '\0';
    return basename_buffer;
}

char* dirname(char* path) {
    if ( !path )
        return simple_buffer('.');

    size_t len = strlen(path);
    if ( !len )
        return simple_buffer('.');

    /* overwrite trailing slashes with null */
    char* last = path + len - 1;
    while ( last >= path && *last == '/' ) {
        *last = 0;
        --last;
    }

    /* return root if the path contains only slashes */
    if ( !*path )
        return simple_buffer('/');

    /* find dir name part */
    char* rightmost_slash = strrchr(path, '/');
    if ( !rightmost_slash )
        return simple_buffer('.');

    /* null-terminate at end */
    if ( rightmost_slash == path )
        rightmost_slash[1] = '\0';
    else
        rightmost_slash[0] = '\0';
    return path;
}
