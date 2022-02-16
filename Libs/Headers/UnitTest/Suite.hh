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

#pragma once

#include <LibTC/Collection/Vector.hh>
#include <UnitTest/Case.hh>

namespace UnitTest {

class Suite {
public:
    static Suite& inst();
    static void   finalize();

    int main(int argc, char const* const* argv);

    void add_case_to_suite(Case& test_case);

    void current_test_must_fail();

private:
    static Suite* s_instance;

    TC::Collection::Vector<Case*> m_test_cases{};
    bool                          m_current_test_have_failed{ false };
};

} /* namespace UnitTest */