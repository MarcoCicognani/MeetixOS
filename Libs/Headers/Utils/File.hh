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

#pragma once

#include <Api.h>
#include <cstdint>
#include <string>

namespace Utils::File {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Reads from the given file-handle and stores the content into <out>
 */
bool read_string(FileHandle file, std::string& out);

/**
 * @brief Reads <len> bytes from the given file-handle and stores them into <buffer>
 */
bool read_bytes(FileHandle file, u8* buffer, usize len);
bool read_bytes(FileHandle file, usize offset, u8* buffer, usize len);

/**
 * @brief Tries to read <len> bytes from the given file-handle and stores them into <buffer>
 */
bool try_read_bytes(FILE* file, usize offset, u8* buffer, usize len);

} /* namespace Utils::File */