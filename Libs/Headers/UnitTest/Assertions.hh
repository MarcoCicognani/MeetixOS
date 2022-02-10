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

#include <stdio.h>
#include <UnitTest/Suite.hh>

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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY(%s) failed\n", __FILE__, __LINE__, #expression);           \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_EQUAL(first, second)                                                                                    \
    do {                                                                                                               \
        if ( !(first == second) ) [[unlikely]] {                                                                       \
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_EQUAL(%s == %s) failed\n",                                 \
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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_FALSE(!%s) failed\n", __FILE__, __LINE__, #expression);    \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_GREATER(first, second)                                                                                  \
    do {                                                                                                               \
        if ( !(first > second) ) [[unlikely]] {                                                                        \
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_GREATER(%s > %s) failed\n",                                \
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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_GREATER_EQUAL(%s >= %s) failed\n",                         \
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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_LESS(%s < %s) failed\n",                                   \
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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_LESS_EQUAL(%s <= %s) failed\n",                            \
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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_NOT_EQUAL(%s != %s) failed\n",                             \
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
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_NOT_NULL(%s != nullptr) failed\n",                         \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #expression);                                                                                       \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )

#define VERIFY_NULL(expression)                                                                                        \
    do {                                                                                                               \
        if ( !(expression == nullptr) ) [[unlikely]] {                                                                 \
            printf("\t\033[31mFAILURE\033[0m %s:%d - VERIFY_NULL(%s == nullptr) failed\n",                             \
                   __FILE__,                                                                                           \
                   __LINE__,                                                                                           \
                   #expression);                                                                                       \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )