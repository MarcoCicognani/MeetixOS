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

#include "Tests/Threads/Native.hh"

namespace Threads {

Native::Native(bool is_verbose)
    : Test{ is_verbose } {
}

bool Native::run() {
    return false;
}

const char* Native::name() const {
    return nullptr;
}

} /* namespace Threads */