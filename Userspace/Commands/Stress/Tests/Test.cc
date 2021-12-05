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

#include "Tests/Test.hh"

#include <iostream>

Test::Test(bool is_verbose)
    : m_is_verbose{ is_verbose } {
}

bool Test::execute() {
    std::cout << "Executing '" << name() << "' Test...\n";

    /* run this test */
    auto tick_begin   = s_millis();
    auto test_success = run();
    auto tick_end     = s_millis();

    if ( test_success )
        std::cout << "\033[;32mSUCCESS\033[0m";
    else
        std::cout << "\033[;31mFAILED\033[0m";

    std::cout << ": In " << (tick_end - tick_begin) << " milliseconds" << std::endl;
    return test_success;
}
