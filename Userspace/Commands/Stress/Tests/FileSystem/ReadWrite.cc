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

#include "Tests/FileSystem/ReadWrite.hh"

#include <cstring>
#include <fcntl.h>

namespace Tests::FileSystem {

const char* ReadWrite::category() const {
    return "FileSystem";
}

const char* ReadWrite::name() const {
    return "ReadWrite";
}

bool ReadWrite::run() {
    static constexpr auto C_FILENAME_1    = "/TestFile_1.txt";
    static constexpr auto C_TEXT_TO_WRITE = "Hello world!!!";

    FsOpenStatus open_status{};
    std::string  file_path{ tests_home() + C_FILENAME_1 };

    /* open the file at the given path */
    auto file_handle = s_open_fms(file_path.c_str(), O_RDWR, 0, &open_status);
    if ( open_status != FS_OPEN_SUCCESSFUL ) {
        logger() << "Failed to open '" << file_path << "' with error " << open_status << '\n';
        return false;
    }

    const auto text_to_write_len = strlen(C_TEXT_TO_WRITE) + 1;

    /* write into it the text to write */
    FsWriteStatus write_status{};
    auto          written_bytes = s_write_s(file_handle, C_TEXT_TO_WRITE, text_to_write_len, &write_status);
    if ( write_status != FS_WRITE_SUCCESSFUL || written_bytes != text_to_write_len ) {
        logger() << "Failed to write '" << file_path << "' with error " << write_status << '\n';
        return false;
    }

    /* read the content again */
    FsReadStatus read_status{};
    u8           out_buffer[text_to_write_len];
    auto         read_bytes = s_read_s(file_handle, out_buffer, text_to_write_len, &read_status);
    if ( read_status != FS_READ_SUCCESSFUL || read_bytes != text_to_write_len ) {
        logger() << "Failed to read '" << file_path << "' with error " << write_status << '\n';
        return false;
    }

    return std::memcmp(C_TEXT_TO_WRITE, out_buffer, text_to_write_len) == 0;
}

} /* namespace Tests::FileSystem */