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

#include <LibTC/Lang/Function.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(construct) {
    Function<i32()> function = []() -> i32 { return 0; };

    VERIFY_EQUAL(function(), 0);
}

TEST_CASE(construct_complex_function) {
    Function<usize(i32, char, usize)> sum_function = [](i32 a, char b, usize c) -> usize { return a + b + c; };

    VERIFY_EQUAL(sum_function(1, 2, 3), 6);
    VERIFY_EQUAL(sum_function(10, 10, 10), 30);
    VERIFY_EQUAL(sum_function(45, 33, 97), 175);
}

TEST_CASE(swap) {
    Function<char()> a_function = []() -> char { return 'a'; };
    VERIFY_EQUAL(a_function(), 'a');

    Function<char()> b_function = []() -> char { return 'b'; };
    VERIFY_EQUAL(b_function(), 'b');

    b_function.swap(a_function);

    VERIFY_EQUAL(a_function(), 'b');
    VERIFY_EQUAL(b_function(), 'a');
}

TEST_CASE(move) {
    Function<i32()> function_0 = []() -> i32 { return 0; };
    VERIFY_EQUAL(function_0(), 0);

    Function<i32()> function_1 = []() -> i32 { return 1; };
    VERIFY_EQUAL(function_1(), 1);

    function_1 = move(function_0);
    VERIFY_EQUAL(function_1(), 0);
}

TEST_CASE(reference_capturing_lambda) {
    int i = 0;

    Function<void()> inc_function = [&i]() { i = 100; };
    inc_function();

    VERIFY_EQUAL(i, 100);
}

TEST_CASE(copy_capturing_lambda) {
    i32 a = 1;
    i32 b = 2;
    i32 c = 3;
    i32 d = 4;
    i32 e = 5;
    i32 f = 6;

    Function<i32()> calc_function = [=]() mutable {
        c = a + b;    /* 3 */
        d = e + f;    /* 11 */
        c *= 2;       /* 6 */
        return d - c; /* 5 */
    };

    VERIFY_EQUAL(calc_function(), 5);
    VERIFY_EQUAL(c, 3);
    VERIFY_EQUAL(d, 4);
}

void ensure_call_equals_with_functor(Function<usize()> function) {
    for ( auto const i : Range{ 0u, 1'000'000u } ) {
        VERIFY_EQUAL(function(), i);
    }
}

BENCHMARK_CASE(call_one_hundred_thousand_times) {
    usize counter = 0;

    ensure_call_equals_with_functor([&counter]() -> usize { return counter++; });
}

void ensure_call_equals_with_template(Callable<usize> auto function) {
    for ( auto const i : Range{ 0u, 1'000'000u } ) {
        VERIFY_EQUAL(function(), i);
    }
}

BENCHMARK_CASE(call_one_hunder_thousand_times) {
    usize counter = 0;

    ensure_call_equals_with_template([&counter]() -> usize { return counter++; });
}