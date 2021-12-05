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

#include "Tests/Memory/Heap.hh"

namespace Memory {

Heap::Heap(bool is_verbose)
    : Test{ is_verbose } {
}

bool Heap::run() {
    return false;
}

const char* Heap::name() const {
    return nullptr;
}

} /* namespace Memory */