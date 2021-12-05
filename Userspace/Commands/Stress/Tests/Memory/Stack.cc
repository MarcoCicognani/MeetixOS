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

#include "Tests/Memory/Stack.hh"

namespace Memory {

Memory::Stack::Stack(bool is_verbose)
    : Test{ is_verbose } {
}

bool Stack::run() {
    return false;
}

const char* Stack::name() const {
    return nullptr;
}

} /* namespace Memory */