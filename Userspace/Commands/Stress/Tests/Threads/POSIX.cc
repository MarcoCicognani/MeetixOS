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

#include "Tests/Threads/POSIX.hh"

namespace Threads {

POSIX::POSIX(bool is_verbose)
    : Test{ is_verbose } {
}

bool POSIX::run() {
    return false;
}

const char* POSIX::name() const {
    return nullptr;
}

} /* namespace Threads */