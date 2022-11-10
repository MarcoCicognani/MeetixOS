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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibC/libgen.h>
#include <LibC/string.h>
#include <LibLS/Core/Assertions.hh>
#include <LibLS/Lang/StringView.hh>

static char s_dot[]   = ".";
static char s_slash[] = "/";

extern "C" {

char* dirname(char* path) {
    if ( path == nullptr )
        return s_dot;

    /* clean all the trailing slashes */
    auto len = strlen(path);
    while ( len > 1 && path[len - 1] == '/' ) {
        path[len - 1] = '\0';
        len--;
    }

    auto path_sv = StringView::construct_from_raw_parts(path, len);

    /* find the last slash */
    auto last_slash_index_or_none = path_sv.find_last('/');
    if ( !last_slash_index_or_none.is_present() )
        return s_dot;

    auto const last_slash_index = last_slash_index_or_none.unwrap();
    if ( last_slash_index == 0 )
        return s_slash;

    /* null terminate the path buffer given at the last slash */
    path[last_slash_index] = '\0';
    return path;
}

char* basename(char* path) {
    if ( path == nullptr )
        return s_dot;

    /* clean all the trailing slashes */
    auto len = strlen(path);
    while ( len > 1 && path[len - 1] == '/' ) {
        path[len - 1] = '\0';
        len--;
    }

    auto path_sv = StringView::construct_from_raw_parts(path, len);

    /* find the last slash */
    auto last_slash_index_or_none = path_sv.find_last('/');
    if ( !last_slash_index_or_none.is_present() )
        return path;

    auto const last_slash_index = last_slash_index_or_none.unwrap();
    if ( len == 1 ) {
        verify_equal$(last_slash_index, 0);
        verify_equal$(path[0], '/');

        return s_slash;
    }

    return path + last_slash_index + 1;
}

} /* extern "C" */

#pragma clang diagnostic pop
