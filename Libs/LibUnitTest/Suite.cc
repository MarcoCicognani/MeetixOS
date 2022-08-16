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
#include <LibFmtIO/Out.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibUnitTest/Suite.hh>

namespace UnitTest {

static Option<NonNullRef<Suite>> s_instance = None;

auto Suite::inst() -> NonNullRef<Suite> {
    if ( !s_instance.is_present() )
        s_instance = NonNullRef<Suite>::construct_from_args();

    return s_instance.value().clone();
}

auto Suite::run(Vector<StringView> args) -> ErrorOr<void> {
    usize test_completed      = 0;
    usize test_failed         = 0;
    usize benchmark_completed = 0;
    usize benchmark_failed    = 0;

    FmtIO::outln("- Starting test suite {}..."sv, args[0], m_test_cases.count());
    auto const all_tests_start_timestamp = s_millis();
    for ( auto const* test_case : m_test_cases ) {
        if ( args.count() > 1 && !args.find(test_case->name()).is_present() )
            continue;

        m_current_test_have_failed = false;

        auto const test_type = test_case->is_benchmark() ? "Benchmark"sv : "Test"sv;
        FmtIO::outln("{} - {} - Running"sv, test_case->name(), test_type);

        /* run the test */
        auto const start_timestamp = s_millis();
        test_case->run_test();
        auto const end_timestamp = s_millis();

        auto const exec_time = end_timestamp - start_timestamp;
        FmtIO::outln("\t{} in {} ms"sv, m_current_test_have_failed ? "\033[31mFailed\033[0m"sv : "\033[32mCompleted\033[0m"sv, exec_time);

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

    FmtIO::outln("{} - Executed \033[32m{}\033[0m tests (\033[32m{}\033[0m tests/\033[32m{}\033[0m benchmarks) in {} ms"sv,
                 args[0],
                 m_test_cases.count(),
                 test_completed + test_failed,
                 benchmark_completed + benchmark_failed,
                 all_tests_end_timestamp - all_tests_start_timestamp);
    if ( test_completed + test_failed > 0 )
        FmtIO::outln("\tTests      - \033[32m{:3}\033[0m Completed/\033[31m{:3}\033[0m Failed"sv, test_completed, test_failed);
    if ( benchmark_completed + benchmark_failed > 0 )
        FmtIO::outln("\tBenchmarks - \033[32m{:3}\033[0m Completed/\033[31m{:3}\033[0m Failed"sv, benchmark_completed, benchmark_failed);

    if ( test_failed != 0 || benchmark_failed != 0 )
        return Error{ "Some tests are failed" };
    else
        return {};
}

void Suite::current_test_must_fail() {
    m_current_test_have_failed = true;
}

void Suite::add_case_to_suite(Case& test_case) {
    m_test_cases.append(&test_case);
}

constexpr Suite::Suite() noexcept
    : m_test_cases{ Vector<Case*>::construct_empty() } {
}

} /* namespace UnitTest */
