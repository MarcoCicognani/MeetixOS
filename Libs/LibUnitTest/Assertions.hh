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

#include <LibUnitTest/Suite.hh>
#include <stdio.h>

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

#define VERIFY(expression)                                                                                             \
    do {                                                                                                               \
        if ( !static_cast<bool>(expression) ) [[unlikely]] {                                                           \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY(\033[31m%s\033[0m) Failed...\n",                     \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #expression);                                                                                       \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_EQUAL(first, second)                                                                                    \
    do {                                                                                                               \
        if ( !(first == second) ) [[unlikely]] {                                                                       \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_EQUAL(\033[31m%s == %s\033[0m) Failed...\n",         \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #first,                                                                                             \
                   #second);                                                                                           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_FALSE(expression)                                                                                       \
    do {                                                                                                               \
        if ( static_cast<bool>(expression) ) [[unlikely]] {                                                            \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_FALSE(\033[31m!%s\033[0m) Failed...\n",              \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #expression);                                                                                       \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_GREATER(first, second)                                                                                  \
    do {                                                                                                               \
        if ( !(first > second) ) [[unlikely]] {                                                                        \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_GREATER(\033[31m%s > %s\033[0m) Failed...\n",        \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #first,                                                                                             \
                   #second);                                                                                           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_GREATER_EQUAL(first, second)                                                                            \
    do {                                                                                                               \
        if ( !(first >= second) ) [[unlikely]] {                                                                       \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_GREATER_EQUAL(\033[31m%s >= %s\033[0m) Failed...\n", \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #first,                                                                                             \
                   #second);                                                                                           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_LESS(first, second)                                                                                     \
    do {                                                                                                               \
        if ( !(first < second) ) [[unlikely]] {                                                                        \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_LESS(\033[31m%s < %s\033[0m) Failed...\n",           \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #first,                                                                                             \
                   #second);                                                                                           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_LESS_EQUAL(first, second)                                                                               \
    do {                                                                                                               \
        if ( !(first <= second) ) [[unlikely]] {                                                                       \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_LESS_EQUAL(\033[31m%s <= %s\033[0m) Failed...\n",    \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #first,                                                                                             \
                   #second);                                                                                           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_NOT_EQUAL(first, second)                                                                                \
    do {                                                                                                               \
        if ( !(first != second) ) [[unlikely]] {                                                                       \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_NOT_EQUAL(\033[31m%s != %s\033[0m) Failed...\n",     \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #first,                                                                                             \
                   #second);                                                                                           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_NOT_NULL(expression)                                                                                    \
    do {                                                                                                               \
        if ( !(expression != nullptr) ) [[unlikely]] {                                                                 \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_NOT_NULL(\033[31m%s != nullptr\033[0m) Failed...\n", \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #expression);                                                                                       \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_NULL(expression)                                                                                        \
    do {                                                                                                               \
        if ( !(expression == nullptr) ) [[unlikely]] {                                                                 \
            printf("\t\033[31mFAILURE\033[0m in %s:%d\n\t\tVERIFY_NULL(\033[31m%s == nullptr\033[0m) Failed...\n",     \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #expression);                                                                                       \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )
