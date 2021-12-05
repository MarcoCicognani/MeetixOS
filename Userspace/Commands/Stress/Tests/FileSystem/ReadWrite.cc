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

#include "Tests/FileSystem/ReadWrite.hh"

namespace FileSystem {

ReadWrite::ReadWrite(bool is_verbose)
    : Test{ is_verbose } {
}

bool ReadWrite::run() {
    return false;
}

const char* ReadWrite::name() const {
    return nullptr;
}

} /* namespace FileSystem */