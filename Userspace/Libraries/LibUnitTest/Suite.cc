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
#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/StringView.hh>
#include <LibUnitTest/Suite.hh>

namespace UnitTest {

static Option<NonNullRef<Suite>> s_instance = OptionNone;

auto Suite::inst() -> NonNullRef<Suite> {
    if ( !s_instance.is_present() )
        s_instance = must$(NonNullRef<Suite>::try_from_adopt(new (nothrow) Suite()));

    return s_instance.value().clone();
}

auto Suite::run(Vector<StringView> args) -> ErrorOr<void> {
    usize tests_completed = 0;
    usize tests_failed    = 0;
    usize tests_skipped   = 0;

    usize benchmarks_completed = 0;
    usize benchmarks_failed    = 0;
    usize benchmarks_skipped   = 0;

    try$(FmtIO::outln("- Starting test suite {}{}{}...\n"sv, FmtIO::foreground(FmtIO::Color::Yellow), args[0], FmtIO::reset(), m_test_cases.count()));

    auto const all_cases_start_ts = s_millis();
    for ( auto const* test_case : m_test_cases ) {
        /* if tests names are given execute only those are explicitly cited */
        if ( args.count() > 1 && !args.find(test_case->name()).is_present() ) {
            if ( test_case->is_benchmark() )
                ++benchmarks_skipped;
            else
                ++tests_skipped;
            continue;
        }

        m_current_test_have_failed = false;
        try$(FmtIO::outln("{} - {} - Running"sv, test_case->name(), test_case->is_benchmark() ? "Benchmark"sv : "Test"sv));

        /* run the test */
        auto const case_start_ts = s_millis();
        test_case->run_test();
        auto const case_end_ts = s_millis();

        auto const case_exec_time = case_end_ts - case_start_ts;
        if ( m_current_test_have_failed )
            try$(FmtIO::outln("\t{}Failed{} in {} ms"sv, FmtIO::foreground(FmtIO::Color::Red), FmtIO::reset(), case_exec_time));
        else
            try$(FmtIO::outln("\t{}Completed{} in {} ms"sv, FmtIO::foreground(FmtIO::Color::Green), FmtIO::reset(), case_exec_time));

        if ( m_current_test_have_failed ) {
            if ( test_case->is_benchmark() )
                ++benchmarks_failed;
            else
                ++tests_failed;
        } else {
            if ( test_case->is_benchmark() )
                ++benchmarks_completed;
            else
                ++tests_completed;
        }
    }
    auto const all_cases_end_ts = s_millis();

    auto const all_tests_time       = all_cases_end_ts - all_cases_start_ts;
    auto const tests_executed       = tests_completed + tests_failed;
    auto const benchmarks_executed  = benchmarks_completed + benchmarks_failed;
    auto const total_cases_executed = tests_executed + benchmarks_executed;

    try$(FmtIO::outln("\n- Executed {}{}/{}{} cases ({}{}{}/{}{}{} tests {}{}{}/{}{}{} benchmarks) in {} ms"sv,
                     FmtIO::foreground(FmtIO::Color::Green),
                     total_cases_executed,
                     m_test_cases.count(),
                     FmtIO::reset(),
                     FmtIO::foreground(FmtIO::Color::Green),
                     tests_executed,
                     FmtIO::reset(),
                     FmtIO::foreground(FmtIO::Color::Yellow),
                     tests_skipped,
                     FmtIO::reset(),
                     FmtIO::foreground(FmtIO::Color::Green),
                     benchmarks_executed,
                     FmtIO::reset(),
                     FmtIO::foreground(FmtIO::Color::Yellow),
                     benchmarks_skipped,
                     FmtIO::reset(),
                     all_tests_time));
    if ( tests_executed > 0 ) {
        try$(FmtIO::outln("- Tests      - {}{:3}{} Completed | {}{:3}{} Failed | {:3} Skipped"sv,
                         FmtIO::foreground(FmtIO::Color::Green),
                         tests_completed,
                         FmtIO::reset(),
                         FmtIO::foreground(FmtIO::Color::Red),
                         tests_failed,
                         FmtIO::reset(),
                         tests_skipped));
    }
    if ( benchmarks_executed > 0 ) {
        try$(FmtIO::outln("- Benchmarks - {}{:3}{} Completed | {}{:3}{} Failed | {:3} Skipped"sv,
                         FmtIO::foreground(FmtIO::Color::Green),
                         benchmarks_completed,
                         FmtIO::reset(),
                         FmtIO::foreground(FmtIO::Color::Red),
                         benchmarks_failed,
                         FmtIO::reset(),
                         benchmarks_skipped));
    }

    if ( tests_failed != 0 || benchmarks_failed != 0 )
        return Error::construct_from_literal("Not all the cases have successfully completed");
    else
        return {};
}

void Suite::current_test_must_fail() {
    m_current_test_have_failed = true;
}

void Suite::add_case_to_suite(Case& test_case) {
    m_test_cases.append(&test_case);
}

} /* namespace UnitTest */
