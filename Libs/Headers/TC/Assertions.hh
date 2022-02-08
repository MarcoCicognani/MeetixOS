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

#ifdef IN_KERNEL
#    include <runtime/constructors.hpp>

#    define VERIFY(expression)                  ((void)0)
#    define VERIFY_EQUAL(first, second)         ((void)0)
#    define VERIFY_FALSE(expression)            ((void)0)
#    define VERIFY_GREATER(first, second)       ((void)0)
#    define VERIFY_GREATER_EQUAL(first, second) ((void)0)
#    define VERIFY_LESS(first, second)          ((void)0)
#    define VERIFY_LESS_EQUAL(first, second)    ((void)0)
#    define VERIFY_NOT_EQUAL(first, second)     ((void)0)
#    define VERIFY_NOT_NULL(expression)         ((void)0)
#    define VERIFY_NULL(expression)             ((void)0)
#    define VERIFY_NOT_REACHED()                ((void)0)
#else
#    include <assert.h>

#    define VERIFY(expression)                  assert(expression)
#    define VERIFY_EQUAL(first, second)         assert(first == second)
#    define VERIFY_FALSE(expression)            assert(!expression)
#    define VERIFY_GREATER(first, second)       assert(first > second)
#    define VERIFY_GREATER_EQUAL(first, second) assert(first >= second)
#    define VERIFY_LESS(first, second)          assert(first < second)
#    define VERIFY_LESS_EQUAL(first, second)    assert(first <= second)
#    define VERIFY_NOT_EQUAL(first, second)     assert(first != second)
#    define VERIFY_NOT_NULL(expression)         assert(expression != nullptr)
#    define VERIFY_NULL(expression)             assert(expression == nullptr)
#    define VERIFY_NOT_REACHED()                assert(false)
#endif
