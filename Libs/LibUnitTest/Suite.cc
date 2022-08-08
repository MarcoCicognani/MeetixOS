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

#include <Api/User.h>
#include <LibUnitTest/Suite.hh>
#include <stdio.h>
#include <stdlib.h>

namespace UnitTest {

/* REM use TC::Memory::Box<Suite> */
Suite* Suite::s_instance = nullptr;

Suite& Suite::inst() {
    if ( s_instance == nullptr )
        s_instance = new Suite{};
    return *s_instance;
}

void Suite::finalize() {
    delete s_instance;
}

int Suite::main(int, char const* const* argv) {
    usize test_completed      = 0;
    usize test_failed         = 0;
    usize benchmark_completed = 0;
    usize benchmark_failed    = 0;

    auto all_tests_start_timestamp = s_millis();
    for ( auto test_case : m_test_cases ) {
        m_current_test_have_failed = false;

        auto test_type = test_case->is_benchmark() ? "Benchmark" : "Test";

        printf("%s - %s - Running...\n", test_case->name(), test_type);

        /* run the test */
        auto start_timestamp = s_millis();
        test_case->run_test();
        auto end_timestamp = s_millis();

        auto exec_time = end_timestamp - start_timestamp;
        printf("\t%s in %llu ms\n", m_current_test_have_failed ? "\033[31mFailed\033[0m" : "\033[32mCompleted\033[0m", exec_time);

        if ( m_current_test_have_failed ) {
            if ( test_case->is_benchmark() )
                ++benchmark_failed;
            else
                ++test_failed;
        } else {
            if ( test_case->is_benchmark() )
                ++benchmark_completed;
            else
                ++test_completed;
        }
    }
    auto all_tests_end_timestamp = s_millis();

    printf("%s - Executed \033[32m%lu\033[0m tests (\033[32m%lu\033[0m tests/\033[32m%lu\033[0m benchmarks) in %llums\n",
           argv[0],
           m_test_cases.count(),
           test_completed + test_failed,
           benchmark_completed + benchmark_failed,
           all_tests_end_timestamp - all_tests_start_timestamp);
    if ( test_completed + test_failed > 0 )
        printf("\tTests      - \033[32m%lu\033[0m Completed/\033[31m%lu\033[0m Failed\n", test_completed, test_failed);
    if ( benchmark_completed + benchmark_failed > 0 )
        printf("\tBenchmarks - \033[32m%lu\033[0m Completed/\033[31m%lu\033[0m Failed\n", benchmark_completed, benchmark_failed);

    if ( test_failed != 0 || benchmark_failed != 0 )
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}

void Suite::current_test_must_fail() {
    m_current_test_have_failed = true;
}

void Suite::add_case_to_suite(Case& test_case) {
    m_test_cases.append(&test_case);
}

constexpr Suite::Suite()
    : m_test_cases{ TC::Vector<Case*>::construct_empty() } {
}

} /* namespace UnitTest */
