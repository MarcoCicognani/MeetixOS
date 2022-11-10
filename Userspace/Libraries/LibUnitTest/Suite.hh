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

#pragma once

#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <LibUnitTest/Case.hh>

namespace UnitTest {

class Suite {
    TCNonNullRefConstructible$(Suite);

public:
    /**
     * @brief Singleton provider
     */
    static auto inst() -> NonNullRef<Suite>;

    /**
     * @brief Runs the registered tests
     */
    auto run(Vector<StringView> args) -> ErrorOr<void>;

    /**
     * @brief Adds the given <test_case> to the tests to execute
     */
    auto add_case_to_suite(Case& test_case) -> void;

    /**
     * @brief Called by the overloaded <VERIFY_> macros when the assertion fails
     */
    auto current_test_must_fail() -> void;

private:
    explicit constexpr Suite() = default;

private:
    Vector<Case*> m_test_cases{ Vector<Case*>::construct_empty() };
    bool          m_current_test_have_failed{ false };
};

} /* namespace UnitTest */