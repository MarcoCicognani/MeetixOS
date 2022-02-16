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

#include <LibUnitTest/Case.hh>
#include <LibUnitTest/Suite.hh>

namespace UnitTest {

Case::Case(const char* name, bool is_benchmark)
    : m_name{ name }
    , m_is_benchmark{ is_benchmark } {
    Suite::inst().add_case_to_suite(*this);
}

char const* Case::name() const {
    return m_name;
}

bool Case::is_benchmark() const {
    return m_is_benchmark;
}

} /* namespace UnitTest */
