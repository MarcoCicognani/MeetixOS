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

#undef VERIFY_NQ
#define VERIFY_NQ(first, second)                                                                                       \
    do {                                                                                                               \
        if ( !(first != second) ) [[unlikely]] {                                                                       \
            fprintf(stderr,                                                                                            \
                    "\t\033[31mFAILURE\033[0m %s:%d - VERIFY_NQ(%s != %s) failed\n",                                   \
                    __FILE__,                                                                                          \
                    __LINE__,                                                                                          \
                    #first,                                                                                            \
                    #second);                                                                                          \
            UnitTest::Suite::inst().current_test_must_fail();                                                          \
        }                                                                                                              \
    } while ( false )