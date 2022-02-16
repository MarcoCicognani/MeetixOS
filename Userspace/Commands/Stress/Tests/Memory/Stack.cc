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

#include "Tests/Memory/Stack.hh"

#include <Api/Memory.h>

namespace Tests::Memory {

const char* Stack::category() const {
    return "Memory";
}

const char* Stack::name() const {
    return "Stack";
}

bool Stack::run() {
    return sub_run();
}

bool Stack::sub_run() {
    static constexpr auto C_LOOP_COUNT = THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES - 1;

    static usize s_loop_count = 0;

    logger() << "Allocating 4KiB of stack, loop n" << s_loop_count << std::endl;
    char buffer[PAGE_SIZE]{ '\0' };
    for ( auto i = 0; i < PAGE_SIZE; ++i )
        buffer[i] = static_cast<char>('a' + i);

    if ( s_loop_count++ < C_LOOP_COUNT )
        return sub_run();
    else
        return true;
}

} /* namespace Tests::Memory */