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

#include <LibUtils/File.hh>
#include <sstream>

namespace Utils::File {

bool read_string(FileHandle file, std::string& out) {
    u8                c;
    std::stringstream s;
    while ( s_read(file, &c, 1) > 0 ) {
        /* stop on null-terminator */
        if ( !c ) {
            out = s.str();
            return true;
        }

        s << static_cast<char>(c);
    }

    /* must have a null-terminator */
    return false;
}

bool read_bytes(FileHandle file, u8* buffer, usize len) {
    auto         remain = len;
    FsReadStatus read_status;
    while ( remain ) {
        auto read = s_read_s(file, &buffer[len - remain], remain, &read_status);
        if ( read_status != FS_READ_SUCCESSFUL || read <= 0 )
            return false;
        remain -= read;
    }

    return true;
}

bool read_bytes(FileHandle file, usize offset, u8* buffer, usize len) {
    /* seek before the read */
    FsSeekStatus seek_status;
    s_seek_s(file, offset, FS_SEEK_SET, &seek_status);
    if ( seek_status != FS_SEEK_SUCCESSFUL )
        return false;

    /* read now the content */
    return read_bytes(file, buffer, len);
}

bool try_read_bytes(FILE* file, usize offset, u8* buffer, usize len) {
    auto left = len;
    fseek(file, static_cast<long>(offset), SEEK_SET);

    while ( left ) {
        auto read = fread(&buffer[len - left], 1, left, file);
        if ( !read )
            return false;
        left -= read;
    }
    return true;
}

} /* namespace Utils::File */