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

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Lang/StringView.hh>
#include <LibFmtIO/Err.hh>
#include <LibUnitTest/Suite.hh>

#undef verify_equal_with_msg$
#undef verify_not_equal_with_msg$
#undef verify_greater_with_msg$
#undef verify_greater_equal_with_msg$
#undef verify_less_with_msg$
#undef verify_less_equal_with_msg$
#undef verify_not_reached$

#undef verify_with_msg$
#undef verify_false_with_msg$
#undef verify_null_with_msg$
#undef verify_not_null_with_msg$

#undef verify_equal$
#undef verify_not_equal$
#undef verify_greater$
#undef verify_greater_equal$
#undef verify_less$
#undef verify_less_equal$

#undef verify$
#undef verify_false$
#undef verify_null$
#undef verify_not_null$

#define verify_equal$(lhs, rhs)                                                                                                                                \
    do {                                                                                                                                                       \
        if ( !(lhs == rhs) ) [[unlikely]] {                                                                                                                    \
            FmtIO::errln("\t{}VerifyEqual Failed{} in {}\n\t> {{ {}{} == {}{} }}"sv,                                                                           \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_not_equal$(lhs, rhs)                                                                                                                            \
    do {                                                                                                                                                       \
        if ( !(lhs != rhs) ) [[unlikely]] {                                                                                                                    \
            FmtIO::errln("\t{}VerifyNotEqual Failed{} in {}\n\t> {{ {}{} != {}{} }}"sv,                                                                        \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_greater$(lhs, rhs)                                                                                                                              \
    do {                                                                                                                                                       \
        if ( !(lhs > rhs) ) [[unlikely]] {                                                                                                                     \
            FmtIO::errln("\t{}VerifyGreater Failed{} in {}\n\t> {{ {}{} > {}{} }}"sv,                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_greater_equal$(lhs, rhs)                                                                                                                        \
    do {                                                                                                                                                       \
        if ( !(lhs >= rhs) ) [[unlikely]] {                                                                                                                    \
            FmtIO::errln("\t{}VerifyGreaterEqual Failed{} in {}\n\t> {{ {}{} >= {}{} }}"sv,                                                                    \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_less$(lhs, rhs)                                                                                                                                 \
    do {                                                                                                                                                       \
        if ( !(lhs < rhs) ) [[unlikely]] {                                                                                                                     \
            FmtIO::errln("\t{}VerifyLess Failed{} in {}\n\t> {{ {}{} < {}{} }}"sv,                                                                             \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_less_equal$(lhs, rhs)                                                                                                                           \
    do {                                                                                                                                                       \
        if ( !(lhs <= rhs) ) [[unlikely]] {                                                                                                                    \
            FmtIO::errln("\t{}VerifyLessEqual Failed{} in {}\n\t> {{ {}{} <= {}{} }}"sv,                                                                       \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify$(expr)          verify_equal$(expr, true)
#define verify_false$(expr)    verify_equal$(expr, false)
#define verify_null$(expr)     verify_equal$(expr, nullptr)
#define verify_not_null$(expr) verify_not_equal$(expr, nullptr)

#define verify_is_present$(expr)                                                                                                                               \
    do {                                                                                                                                                       \
        auto value_or_none = expr;                                                                                                                             \
        if ( !value_or_none.is_present() ) [[unlikely]] {                                                                                                      \
            FmtIO::errln("\t{}VerifyIsPresent Failed{} in {}\n\t> {{ {}{}.is_present(){} }}"sv,                                                                \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expr),                                                                                                                \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_is_present_equal$(expr, value)                                                                                                                  \
    do {                                                                                                                                                       \
        auto value_or_none = expr;                                                                                                                             \
        if ( !value_or_none.is_present() || !(value_or_none.unwrap() == value) ) [[unlikely]] {                                                                \
            FmtIO::errln("\t{}VerifyIsPresentEqual Failed{} in {}\n\t> {{ {}{} == Some({}){} }}"sv,                                                            \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expr),                                                                                                                \
                         as_string_view$(value),                                                                                                               \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_is_none$(expr)                                                                                                                                  \
    do {                                                                                                                                                       \
        auto value_or_none = expr;                                                                                                                             \
        if ( value_or_none.is_present() ) [[unlikely]] {                                                                                                       \
            FmtIO::errln(FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         "\t{}VerifyIsNone Failed{} in {}\n\t> {{ {}!{}.is_present(){} }}"sv,                                                                  \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expr),                                                                                                                \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_is_value$(expr)                                                                                                                                 \
    do {                                                                                                                                                       \
        auto error_or_value = expr;                                                                                                                            \
        if ( !error_or_value.is_value() ) [[unlikely]] {                                                                                                       \
            FmtIO::errln(FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         "\t{}VerifyIsValue Failed{} in {}\n\t> {{ {}{}.is_value(){} }}"sv,                                                                    \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expr),                                                                                                                \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_is_value_equal$(expr, value)                                                                                                                    \
    do {                                                                                                                                                       \
        auto error_or_value = expr;                                                                                                                            \
        if ( !error_or_value.is_value() || !(error_or_value.unwrap() == value) ) [[unlikely]] {                                                                \
            FmtIO::errln("\t{}VerifyIsValueEqual Failed{} in {}\n\t> {{ {}{} == Value({}){} }}"sv,                                                             \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expr),                                                                                                                \
                         as_string_view$(value),                                                                                                               \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_is_error_equal$(expr, error)                                                                                                                    \
    do {                                                                                                                                                       \
        auto error_or_value = expr;                                                                                                                            \
        if ( !error_or_value.is_error() || !(error_or_value.unwrap_error() == error) ) [[unlikely]] {                                                          \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == Error({}){} }}"sv,                                                                         \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expr),                                                                                                                \
                         as_string_view$(error),                                                                                                               \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )