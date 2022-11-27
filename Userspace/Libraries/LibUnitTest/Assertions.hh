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

#include <CCLang/Lang/StringView.hh>
#include <LibFmtIO/Err.hh>
#include <LibUnitTest/Suite.hh>

#undef verify$
#undef verify_equal$
#undef verify_false$
#undef verify_greater$
#undef verify_greater_equal$
#undef verify_less$
#undef verify_less_equal$
#undef verify_not_equal$
#undef verify_not_null$
#undef verify_null$
#undef VERIFY_IS_PRESENT_EQUAL
#undef VERIFY_IS_NONE
#undef VERIFY_IS_VALUE
#undef VERIFY_IS_VALUE_EQUAL
#undef VERIFY_IS_ERROR_EQUAL

#define verify$(expression)                                                                                                                                    \
    do {                                                                                                                                                       \
        if ( !static_cast<bool>(expression) ) [[unlikely]] {                                                                                                   \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} != false{} }}"sv,                                                                             \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_equal$(lhs, rhs)                                                                                                                                \
    do {                                                                                                                                                       \
        if ( !((lhs) == (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == {}{} }}"sv,                                                                                \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_false$(expression)                                                                                                                              \
    do {                                                                                                                                                       \
        if ( static_cast<bool>(expression) ) [[unlikely]] {                                                                                                    \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == false{} }}"sv,                                                                             \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_greater$(lhs, rhs)                                                                                                                              \
    do {                                                                                                                                                       \
        if ( !((lhs) > (rhs)) ) [[unlikely]] {                                                                                                                 \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} > {}{} }}"sv,                                                                                 \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
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
        if ( !((lhs) >= (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} >= {}{} }}"sv,                                                                                \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
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
        if ( !((lhs) < (rhs)) ) [[unlikely]] {                                                                                                                 \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} < {}{} }}"sv,                                                                                 \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
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
        if ( !((lhs) <= (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} <= {}{} }}"sv,                                                                                \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
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
        if ( !((lhs) != (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} != {}{} }}"sv,                                                                                \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(lhs),                                                                                                                 \
                         as_string_view$(rhs),                                                                                                                 \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_not_null$(expression)                                                                                                                           \
    do {                                                                                                                                                       \
        if ( !((expression) != nullptr) ) [[unlikely]] {                                                                                                       \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} != nullptr{} }}"sv,                                                                           \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define verify_null$(expression)                                                                                                                               \
    do {                                                                                                                                                       \
        if ( !((expression) == nullptr) ) [[unlikely]] {                                                                                                       \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == nullptr{} }}"sv,                                                                           \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_PRESENT(expression)                                                                                                                          \
    do {                                                                                                                                                       \
        auto value_or_none = (expression);                                                                                                                     \
        if ( !value_or_none.is_present() ) [[unlikely]] {                                                                                                      \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{}.is_present(){} }}"sv,                                                                         \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_PRESENT_EQUAL(expression, expected_value)                                                                                                    \
    do {                                                                                                                                                       \
        auto value_or_none = (expression);                                                                                                                     \
        if ( !value_or_none.is_present() || !(value_or_none.value() == (expected_value)) ) [[unlikely]] {                                                      \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == Some({}){} }}"sv,                                                                          \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         as_string_view$(expected_value),                                                                                                      \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_NONE(expression)                                                                                                                             \
    do {                                                                                                                                                       \
        auto value_or_none = (expression);                                                                                                                     \
        if ( value_or_none.is_present() ) [[unlikely]] {                                                                                                       \
            FmtIO::errln(FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         "\t{}Verify Failed{} in {}\n\t> {{ {}!{}.is_present(){} }}"sv,                                                                        \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_VALUE(expression)                                                                                                                            \
    do {                                                                                                                                                       \
        auto error_or_value = (expression);                                                                                                                    \
        if ( !error_or_value.is_value() ) [[unlikely]] {                                                                                                       \
            FmtIO::errln(FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         "\t{}Verify Failed{} in {}\n\t> {{ {}{}.is_value(){} }}"sv,                                                                           \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_VALUE_EQUAL(expression, expected_value)                                                                                                      \
    do {                                                                                                                                                       \
        auto error_or_value = (expression);                                                                                                                    \
        if ( !error_or_value.is_value() || !(error_or_value.value() == (expected_value)) ) [[unlikely]] {                                                      \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == Value({}){} }}"sv,                                                                         \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         as_string_view$(expected_value),                                                                                                      \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_ERROR_EQUAL(expression, expected_error)                                                                                                      \
    do {                                                                                                                                                       \
        auto error_or_value = (expression);                                                                                                                    \
        if ( !error_or_value.is_error() || !(error_or_value.error() == (expected_error)) ) [[unlikely]] {                                                      \
            FmtIO::errln("\t{}Verify Failed{} in {}\n\t> {{ {}{} == Error({}){} }}"sv,                                                                         \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         FmtIO::reset(),                                                                                                                       \
                         SourceLocation::from_here(),                                                                                                      \
                         FmtIO::foreground(FmtIO::Color::Red),                                                                                                 \
                         as_string_view$(expression),                                                                                                          \
                         as_string_view$(expected_error),                                                                                                      \
                         FmtIO::reset())                                                                                                                       \
                .unwrap();                                                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )