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

#include "Tests/FileSystem/Create.hh"

namespace FileSystem {

Create::Create(bool is_verbose)
    : Test{ is_verbose } {
}

bool Create::run() {
    return false;
}

const char* Create::name() const {
    return nullptr;
}

} /* namespace FileSystem */
