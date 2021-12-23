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

namespace Tests::Memory {

const char* Stack::category() const {
    return "Memory";
}

const char* Stack::name() const {
    return "Stack";
}

bool Stack::run() {
    return false;
}

} /* namespace Tests::Memory */