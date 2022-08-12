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

#include <LibFmtIO/Err.hh>
#include <LibUnitTest/Suite.hh>

#undef VERIFY
#undef VERIFY_EQUAL
#undef VERIFY_FALSE
#undef VERIFY_GREATER
#undef VERIFY_GREATER_EQUAL
#undef VERIFY_LESS
#undef VERIFY_LESS_EQUAL
#undef VERIFY_NOT_EQUAL
#undef VERIFY_NOT_NULL
#undef VERIFY_NULL
#undef VERIFY_IS_PRESENT_EQUAL
#undef VERIFY_IS_NONE
#undef VERIFY_IS_VALUE
#undef VERIFY_IS_VALUE_EQUAL
#undef VERIFY_IS_ERROR_EQUAL

#define VERIFY(expression)                                                                                                                 \
    do {                                                                                                                                   \
        if ( !static_cast<bool>(expression) ) [[unlikely]] {                                                                               \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY(\033[31m{}\033[0m) Failed...\n"sv,                                 \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_EQUAL(first, second)                                                                                                        \
    do {                                                                                                                                   \
        if ( !((first) == (second)) ) [[unlikely]] {                                                                                       \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_EQUAL(\033[31m{} == {}\033[0m) Failed...\n"sv,                     \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #first,                                                                                                           \
                         #second);                                                                                                         \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_FALSE(expression)                                                                                                           \
    do {                                                                                                                                   \
        if ( static_cast<bool>(expression) ) [[unlikely]] {                                                                                \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_FALSE(\033[31m!{}\033[0m) Failed...\n"sv,                          \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_GREATER(first, second)                                                                                                      \
    do {                                                                                                                                   \
        if ( !((first) > (second)) ) [[unlikely]] {                                                                                        \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_GREATER(\033[31m{} > {}\033[0m) Failed...\n"sv,                    \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #first,                                                                                                           \
                         #second);                                                                                                         \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_GREATER_EQUAL(first, second)                                                                                                \
    do {                                                                                                                                   \
        if ( !((first) >= (second)) ) [[unlikely]] {                                                                                       \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_GREATER_EQUAL(\033[31m{} >= {}\033[0m) Failed...\n"sv,             \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #first,                                                                                                           \
                         #second);                                                                                                         \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_LESS(first, second)                                                                                                         \
    do {                                                                                                                                   \
        if ( !((first) < (second)) ) [[unlikely]] {                                                                                        \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_LESS(\033[31m{} < {}\033[0m) Failed...\n"sv,                       \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #first,                                                                                                           \
                         #second);                                                                                                         \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_LESS_EQUAL(first, second)                                                                                                   \
    do {                                                                                                                                   \
        if ( !((first) <= (second)) ) [[unlikely]] {                                                                                       \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_LESS_EQUAL(\033[31m{} <= {}\033[0m) Failed...\n"sv,                \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #first,                                                                                                           \
                         #second);                                                                                                         \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_NOT_EQUAL(first, second)                                                                                                    \
    do {                                                                                                                                   \
        if ( !((first) != (second)) ) [[unlikely]] {                                                                                       \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_NOT_EQUAL(\033[31m{} != {}\033[0m) Failed...\n"sv,                 \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #first,                                                                                                           \
                         #second);                                                                                                         \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_NOT_NULL(expression)                                                                                                        \
    do {                                                                                                                                   \
        if ( !((expression) != nullptr) ) [[unlikely]] {                                                                                   \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_NOT_NULL(\033[31m{} != nullptr\033[0m) Failed...\n"sv,             \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_NULL(expression)                                                                                                            \
    do {                                                                                                                                   \
        if ( !((expression) == nullptr) ) [[unlikely]] {                                                                                   \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_NULL(\033[31m{} == nullptr\033[0m) Failed...\n"sv,                 \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_IS_PRESENT(expression)                                                                                                      \
    do {                                                                                                                                   \
        auto value_or_none = (expression);                                                                                                 \
        if ( !value_or_none.is_present() ) [[unlikely]] {                                                                                  \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_PRESENT(\033[31m{}\033[0m) Failed...\n"sv,                         \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_IS_PRESENT_EQUAL(expression, expected_value)                                                                                \
    do {                                                                                                                                   \
        auto value_or_none = (expression);                                                                                                 \
        if ( !value_or_none.is_present() || !(value_or_none.value() == (expected_value)) ) [[unlikely]] {                                  \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_PRESENT_EQUAL(\033[31m{} == {}\033[0m) Failed...\n"sv,             \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression,                                                                                                      \
                         #expected_value);                                                                                                 \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_IS_NONE(expression)                                                                                                         \
    do {                                                                                                                                   \
        auto value_or_none = (expression);                                                                                                 \
        if ( value_or_none.is_present() ) [[unlikely]] {                                                                                   \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_NONE(\033[31m{}\033[0m) Failed...\n"sv,                            \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_IS_VALUE(expression)                                                                                                        \
    do {                                                                                                                                   \
        auto error_or_value = (expression);                                                                                                \
        if ( !error_or_value.is_value() ) [[unlikely]] {                                                                                   \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_VALUE(\033[31m{}\033[0m) Failed...\n"sv,                           \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression);                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_IS_VALUE_EQUAL(expression, expected_value)                                                                                  \
    do {                                                                                                                                   \
        auto error_or_value = (expression);                                                                                                \
        if ( !error_or_value.is_value() || !(error_or_value.value() == (expected_value)) ) [[unlikely]] {                                  \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_VALUE_EQUAL(\033[31m{} == {}\033[0m) Failed...\n"sv,               \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression,                                                                                                      \
                         #expected_value);                                                                                                 \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )

#define VERIFY_IS_ERROR_EQUAL(expression, expected_error)                                                                                  \
    do {                                                                                                                                   \
        auto error_or_value = (expression);                                                                                                \
        if ( !error_or_value.is_error() || !(error_or_value.error() == (expected_error)) ) [[unlikely]] {                                  \
            FmtIO::errln("\t\033[31mFAILURE\033[0m in {}:{}\n\t\tVERIFY_ERROR_EQUAL(\033[31m{} == {}\033[0m) Failed...\n"sv,               \
                         __FILE__,                                                                                                         \
                         __LINE__,                                                                                                         \
                         #expression,                                                                                                      \
                         #expected_error);                                                                                                 \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                     \
        }                                                                                                                                  \
    } while ( false )