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

#include <LibTC/Collection/Function.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(construct) {
    Function<int()> function{ []() { return 0; } };

    VERIFY_EQUAL(function(), 0);
}

TEST_CASE(construct_complex_function) {
    Function<long(int, char, long)> sum_function([](int a, char b, long c) { return a + b + c; });

    VERIFY_EQUAL(sum_function(1, 2, 3), 6);
    VERIFY_EQUAL(sum_function(10, 10, 10), 30);
    VERIFY_EQUAL(sum_function(45, 33, 97), 175);
}

TEST_CASE(swap) {
    Function<char()> a_function{ []() { return 'a'; } };
    VERIFY_EQUAL(a_function(), 'a');

    Function<char()> b_function{ []() { return 'b'; } };
    VERIFY_EQUAL(b_function(), 'b');

    b_function.swap(a_function);

    VERIFY_EQUAL(a_function(), 'b');
    VERIFY_EQUAL(b_function(), 'a');
}

TEST_CASE(move) {
    Function<int()> function_0{ []() { return 0; } };
    VERIFY_EQUAL(function_0(), 0);

    Function<int()> function_1{ []() { return 1; } };
    VERIFY_EQUAL(function_1(), 1);

    function_1 = move(function_0);
    VERIFY_EQUAL(function_1(), 0);
}

TEST_CASE(reference_capturing_lambda) {
    int i = 0;

    Function<void()> inc_function{ [&i]() { i = 100; } };
    inc_function();

    VERIFY_EQUAL(i, 100);
}

TEST_CASE(copy_capturing_lambda) {
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    Function<int()> calc_function{ [=]() mutable {
        c = a + b;    /* 3 */
        d = e + f;    /* 11 */
        c *= 2;       /* 6 */
        return d - c; /* 5 */
    } };

    VERIFY_EQUAL(calc_function(), 5);
}

void ensure_call_equals_with_functor(Function<usize()> function) {
    for ( usize i : Range{ 0, 100'000 } ) {
        VERIFY_EQUAL(function(), i);
    }
}

BENCHMARK_CASE(call_one_hundred_thousand_times) {
    usize counter = 0;

    ensure_call_equals_with_functor([&counter]() -> usize { return counter++; });
}

template<Callable<usize> TPredicate>
void ensure_call_equals_with_template(TPredicate function) {
    for ( usize i : Range{ 0, 100'000 } ) {
        VERIFY_EQUAL(function(), i);
    }
}

BENCHMARK_CASE(call_one_hunder_thousand_times) {
    usize counter = 0;

    ensure_call_equals_with_template([&counter]() -> usize { return counter++; });
}