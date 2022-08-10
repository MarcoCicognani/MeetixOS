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

#include <LibTC/Cxx.hh>
#include <LibUnitTest/Case.hh>
#include <LibUnitTest/Suite.hh>

namespace UnitTest {

auto Case::name() const -> StringView {
    return m_name;
}

auto Case::is_benchmark() const -> bool {
    return m_is_benchmark;
}

Case::Case(StringView name, bool is_benchmark) noexcept
    : m_name{ Cxx::move(name) }
    , m_is_benchmark{ is_benchmark } {
    Suite::inst().add_case_to_suite(*this);
}

} /* namespace UnitTest */
