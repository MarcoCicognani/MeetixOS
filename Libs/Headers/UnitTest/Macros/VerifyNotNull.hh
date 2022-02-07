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

#undef VERIFY_NOT_NULL
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