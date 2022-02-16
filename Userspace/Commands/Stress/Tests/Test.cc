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

#include "Tests/Test.hh"

#include <iostream>
#include <LibUtils/Environment.hh>
#include <sstream>

namespace Tests {

bool Test::execute() {
    std::cout << "Executing '" << full_name() << "' Test...\n";

    /* run this test */
    auto tick_begin   = s_millis();
    auto test_success = run();
    auto tick_end     = s_millis();

    if ( test_success )
        std::cout << "\t\033[;32mSUCCESS\033[0m";
    else
        std::cout << "\t\033[;31mFAILED\033[0m";

    std::cout << ": In " << (tick_end - tick_begin) << " milliseconds" << std::endl;
    return test_success;
}

std::string Test::full_name() const {
    std::stringstream ss{};
    ss << category() << '/' << name();
    return ss.str();
}

void Test::enable_verbose() {
    m_is_verbose = true;
}

std::string Test::tests_home() const {
    std::stringstream ss{};
    ss << "/Users/" << Utils::Environment::logged_user() << "/Tests";
    return ss.str();
}

std::ostream& Test::logger() {
    if ( m_is_verbose )
        return std::cout << "\t[" << s_millis() << "ms] " << std::flush;
    else
        return m_null_logger; /* no sense to write */
}

} /* namespace Tests */
