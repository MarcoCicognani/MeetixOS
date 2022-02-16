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

#include "Tests/Threads/POSIX.hh"

namespace Tests::Threads {

const char* POSIX::category() const {
    return "Threads";
}

const char* POSIX::name() const {
    return "POSIX";
}

bool POSIX::run() {
    return false;
}

} /* namespace Tests::Threads */