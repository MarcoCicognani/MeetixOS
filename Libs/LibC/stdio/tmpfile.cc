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

#include <cstdio>

extern "C" FILE* tmpfile() {
    /* obtain temporary filename */
    auto name_buf = new char[L_tmpnam];
    auto name     = tmpnam(name_buf);
    if ( !name ) {
        delete[] name_buf;
        return nullptr;
    }

    /* create the file and return the stream */
    auto stream = fopen(name, "w+");
    delete[] name_buf;
    return stream;
}
