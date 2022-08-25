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
#include <LibTC/Collection/StringView.hh>
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

#define VERIFY(expression)                                                                                                                                     \
    do {                                                                                                                                                       \
        if ( !static_cast<bool>(expression) ) [[unlikely]] {                                                                                                   \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{}\033[0m }} Failed..."sv,                                                     \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_EQUAL(lhs, rhs)                                                                                                                                 \
    do {                                                                                                                                                       \
        if ( !((lhs) == (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} == {}\033[0m }} Failed..."sv,                                               \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((lhs)),                                                                                                             \
                         __EXPRESSION_SV__((rhs)));                                                                                                            \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_FALSE(expression)                                                                                                                               \
    do {                                                                                                                                                       \
        if ( static_cast<bool>(expression) ) [[unlikely]] {                                                                                                    \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m!{}\033[0m }} Failed..."sv,                                                    \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_GREATER(lhs, rhs)                                                                                                                               \
    do {                                                                                                                                                       \
        if ( !((lhs) > (rhs)) ) [[unlikely]] {                                                                                                                 \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} > {}\033[0m }} Failed..."sv,                                                \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((lhs)),                                                                                                             \
                         __EXPRESSION_SV__((rhs)));                                                                                                            \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_GREATER_EQUAL(lhs, rhs)                                                                                                                         \
    do {                                                                                                                                                       \
        if ( !((lhs) >= (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} >= {}\033[0m }} Failed..."sv,                                               \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((lhs)),                                                                                                             \
                         __EXPRESSION_SV__((rhs)));                                                                                                            \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_LESS(lhs, rhs)                                                                                                                                  \
    do {                                                                                                                                                       \
        if ( !((lhs) < (rhs)) ) [[unlikely]] {                                                                                                                 \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} < {}\033[0m }} Failed..."sv,                                                \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((lhs)),                                                                                                             \
                         __EXPRESSION_SV__((rhs)));                                                                                                            \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_LESS_EQUAL(lhs, rhs)                                                                                                                            \
    do {                                                                                                                                                       \
        if ( !((lhs) <= (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} <= {}\033[0m }} Failed..."sv,                                               \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((lhs)),                                                                                                             \
                         __EXPRESSION_SV__((rhs)));                                                                                                            \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_NOT_EQUAL(lhs, rhs)                                                                                                                             \
    do {                                                                                                                                                       \
        if ( !((lhs) != (rhs)) ) [[unlikely]] {                                                                                                                \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} != {}\033[0m }} Failed..."sv,                                               \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((lhs)),                                                                                                             \
                         __EXPRESSION_SV__((rhs)));                                                                                                            \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_NOT_NULL(expression)                                                                                                                            \
    do {                                                                                                                                                       \
        if ( !((expression) != nullptr) ) [[unlikely]] {                                                                                                       \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} != nullptr\033[0m }} Failed..."sv,                                          \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_NULL(expression)                                                                                                                                \
    do {                                                                                                                                                       \
        if ( !((expression) == nullptr) ) [[unlikely]] {                                                                                                       \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} == nullptr\033[0m }} Failed..."sv,                                          \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_PRESENT(expression)                                                                                                                          \
    do {                                                                                                                                                       \
        auto value_or_none = (expression);                                                                                                                     \
        if ( !value_or_none.is_present() ) [[unlikely]] {                                                                                                      \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{}.is_present()\033[0m }} Failed..."sv,                                        \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_PRESENT_EQUAL(expression, expected_value)                                                                                                    \
    do {                                                                                                                                                       \
        auto value_or_none = (expression);                                                                                                                     \
        if ( !value_or_none.is_present() || !(value_or_none.value() == (expected_value)) ) [[unlikely]] {                                                      \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} == Some({})\033[0m }} Failed..."sv,                                         \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)),                                                                                                      \
                         __EXPRESSION_SV__((expected_value)));                                                                                                 \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_NONE(expression)                                                                                                                             \
    do {                                                                                                                                                       \
        auto value_or_none = (expression);                                                                                                                     \
        if ( value_or_none.is_present() ) [[unlikely]] {                                                                                                       \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m!{}.is_present()\033[0m }} Failed..."sv,                                       \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_VALUE(expression)                                                                                                                            \
    do {                                                                                                                                                       \
        auto error_or_value = (expression);                                                                                                                    \
        if ( !error_or_value.is_value() ) [[unlikely]] {                                                                                                       \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{}.is_value()\033[0m }} Failed..."sv,                                          \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)));                                                                                                     \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_VALUE_EQUAL(expression, expected_value)                                                                                                      \
    do {                                                                                                                                                       \
        auto error_or_value = (expression);                                                                                                                    \
        if ( !error_or_value.is_value() || !(error_or_value.value() == (expected_value)) ) [[unlikely]] {                                                      \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} == Value({})\033[0m }} Failed..."sv,                                        \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)),                                                                                                      \
                         __EXPRESSION_SV__((expected_value)));                                                                                                 \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )

#define VERIFY_IS_ERROR_EQUAL(expression, expected_error)                                                                                                      \
    do {                                                                                                                                                       \
        auto error_or_value = (expression);                                                                                                                    \
        if ( !error_or_value.is_error() || !(error_or_value.error() == (expected_error)) ) [[unlikely]] {                                                      \
            FmtIO::errln("\t\033[31mFAIL\033[0m in {}:{}\n\t\tVerify {{ \033[31m{} == Error({})\033[0m }} Failed..."sv,                                        \
                         __FILE_SV__,                                                                                                                          \
                         __LINE__,                                                                                                                             \
                         __EXPRESSION_SV__((expression)),                                                                                                      \
                         __EXPRESSION_SV__((expected_error)));                                                                                                 \
            UnitTest::Suite::inst().as_ref().current_test_must_fail();                                                                                         \
        }                                                                                                                                                      \
    } while ( false )