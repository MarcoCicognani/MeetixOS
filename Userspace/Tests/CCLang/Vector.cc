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

#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Lang/Cxx.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(default_constructor) {
    auto const vector = Vector<i32>::construct_empty();

    verify$(vector.is_empty());
    verify_equal$(vector.count(), 0);
}

TEST_CASE(initializer_list) {
    auto const vector = Vector<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    verify_false$(vector.is_empty());
    verify_equal$(vector.count(), 10);
}

TEST_CASE(capacity) {
    class Object {};
    auto const vector = Vector<Object>::construct_with_capacity(512);

    verify_equal$(vector.count(), 0);
    verify$(vector.is_empty());
    verify_equal$(vector.capacity(), 512);
}

TEST_CASE(ensure_capacity) {
    auto vector = Vector<i32>::construct_empty();

    VERIFY_IS_VALUE(vector.try_ensure_capacity(4096));
    verify_greater_equal$(vector.capacity(), 4096);
}

TEST_CASE(clear) {
    auto vector = Vector<i32>::construct_from_list({ 1, 2, 3, 4, 5 });

    vector.clear_keep_capacity();
    verify$(vector.is_empty());
    verify_greater_equal$(vector.capacity(), 5);

    vector.clear();
    verify$(vector.is_empty());
    verify_equal$(vector.capacity(), 0);
}

TEST_CASE(emplace_and_access) {
    auto vector = Vector<i32>::construct_empty();

    VERIFY_IS_VALUE(vector.try_emplace_first(50));

    verify_equal$(vector.first(), 50);
    verify_equal$(vector.last(), 50);

    verify_false$(vector.is_empty());
    verify_equal$(vector.count(), 1);

    VERIFY_IS_VALUE(vector.try_emplace_first(40));
    VERIFY_IS_VALUE(vector.try_emplace_first(30));
    VERIFY_IS_VALUE(vector.try_emplace_first(20));
    VERIFY_IS_VALUE(vector.try_emplace_first(10));
    verify_equal$(vector.count(), 5);

    verify_equal$(vector[0], 10);
    verify_equal$(vector[1], 20);
    verify_equal$(vector[2], 30);
    verify_equal$(vector[3], 40);
    verify_equal$(vector[4], 50);

    VERIFY_IS_VALUE(vector.try_emplace_last(100));
    VERIFY_IS_VALUE(vector.try_emplace_last(200));
    VERIFY_IS_VALUE(vector.try_emplace_last(300));
    VERIFY_IS_VALUE(vector.try_emplace_last(400));
    VERIFY_IS_VALUE(vector.try_emplace_last(500));
    verify_equal$(vector.count(), 10);

    verify_equal$(vector[5], 100);
    verify_equal$(vector[6], 200);
    verify_equal$(vector[7], 300);
    verify_equal$(vector[8], 400);
    verify_equal$(vector[9], 500);
}

TEST_CASE(append) {
    struct USize {
        usize m_value{ 0 };
    };

    auto vector = Vector<USize>::construct_with_capacity(1);

    VERIFY_IS_VALUE(vector.try_append(USize{ 10 }));
    VERIFY_IS_VALUE(vector.try_append(USize{ 20 }));
    VERIFY_IS_VALUE(vector.try_append(USize{ 30 }));
    VERIFY_IS_VALUE(vector.try_append(USize{ 40 }));
    VERIFY_IS_VALUE(vector.try_append(USize{ 50 }));

    verify_false$(vector.is_empty());
    verify_equal$(vector.count(), 5);
    verify_greater_equal$(vector.capacity(), 5);
}

TEST_CASE(prepend) {
    auto vector = Vector<i32>::construct_from_list({ 5, 6, 7, 8, 9 });

    VERIFY_IS_VALUE(vector.try_prepend(4));
    VERIFY_IS_VALUE(vector.try_prepend(3));
    VERIFY_IS_VALUE(vector.try_prepend(2));
    VERIFY_IS_VALUE(vector.try_prepend(1));
    VERIFY_IS_VALUE(vector.try_prepend(0));

    verify_equal$(vector.count(), 10);
    verify_equal$(vector.first(), 0);
    verify_equal$(vector.last(), 9);
    verify_equal$(vector[0], 0);
    verify_equal$(vector[1], 1);
    verify_equal$(vector[2], 2);
    verify_equal$(vector[3], 3);
    verify_equal$(vector[4], 4);
    verify_equal$(vector[5], 5);
    verify_equal$(vector[6], 6);
    verify_equal$(vector[7], 7);
    verify_equal$(vector[8], 8);
    verify_equal$(vector[9], 9);
}

TEST_CASE(insert_at) {
    auto vector = Vector<i32>::construct_from_list({ 100, 200, 300, 600 });

    VERIFY_IS_VALUE(vector.try_insert_at(3, 400));
    VERIFY_IS_VALUE(vector.try_insert_at(4, 500));

    verify_equal$(vector.count(), 6);
    verify_equal$(vector[0], 100);
    verify_equal$(vector[1], 200);
    verify_equal$(vector[2], 300);
    verify_equal$(vector[3], 400);
    verify_equal$(vector[4], 500);
    verify_equal$(vector[5], 600);
}

TEST_CASE(sort) {
    auto vector = Vector<i32>::construct_from_list({ 9, 4, 6, 12, 44, 102, 0, 45 });
    verify_equal$(vector.count(), 8);

    vector.sort([](i32 const& a, i32 const& b) -> i32 { return a > b ? 1 : -1; });

    verify_equal$(vector[0], 0);
    verify_equal$(vector[1], 4);
    verify_equal$(vector[2], 6);
    verify_equal$(vector[3], 9);
    verify_equal$(vector[4], 12);
    verify_equal$(vector[5], 44);
    verify_equal$(vector[6], 45);
    verify_equal$(vector[7], 102);

    vector.sort([](i32 const& a, i32 const& b) -> i32 { return a < b ? 1 : -1; });

    verify_equal$(vector[0], 102);
    verify_equal$(vector[1], 45);
    verify_equal$(vector[2], 44);
    verify_equal$(vector[3], 12);
    verify_equal$(vector[4], 9);
    verify_equal$(vector[5], 6);
    verify_equal$(vector[6], 4);
    verify_equal$(vector[7], 0);
}

TEST_CASE(erase_at) {
    auto vector = Vector<i32>::construct_from_list({ 150, 250, 350, 450, 550 });

    VERIFY_IS_VALUE(vector.erase_at(1));
    verify_equal$(vector.count(), 4);
    verify_equal$(vector[1], 350);

    VERIFY_IS_VALUE(vector.erase_at(3));
    verify_equal$(vector.count(), 3);
    verify_equal$(vector[2], 450);
}

TEST_CASE(erase_first_of) {
    struct USize {
        usize m_value{ 0 };

        auto operator<=>(USize const& rhs) const -> bool = default;
    };

    auto vector = Vector<USize>::construct_from_list({ USize{ 10 }, USize{ 20 }, USize{ 30 }, USize{ 40 }, USize{ 50 } });

    VERIFY_IS_VALUE(vector.erase_first_of(USize{ 30 }));
    verify_equal$(vector.count(), 4);
    verify_equal$(vector[2], USize{ 40 });

    VERIFY_IS_VALUE(vector.erase_first_of(USize{ 10 }));
    verify_equal$(vector.count(), 3);
    verify_equal$(vector.first(), USize{ 20 });
}

TEST_CASE(erase_all_of) {
    auto vector = Vector<i32>::construct_from_list({ 45, 67, 88, 90, 34, 45, 78, 45, 99 });

    VERIFY_IS_VALUE_EQUAL(vector.erase_all_of(45), 3);
    verify_equal$(vector.count(), 6);
    verify_equal$(vector.first(), 67);

    VERIFY_IS_ERROR_EQUAL(vector.erase_all_of(100), ENOENT);
}

TEST_CASE(erase_all_matches) {
    struct USize {
        usize m_value{ 0 };

        auto operator<=>(USize const& rhs) const -> bool = default;
    };

    auto vector = Vector<USize>::construct_from_list({ USize{ 10 }, USize{ 20 }, USize{ 30 }, USize{ 40 }, USize{ 57 } });

    VERIFY_IS_VALUE_EQUAL(vector.erase_all_matches([](USize const& u) -> bool { return u.m_value % 2 == 0; }), 4);
    verify_equal$(vector.first(), USize{ 57 });
    verify_equal$(vector.count(), 1);
}

TEST_CASE(const_iterator) {
    auto const vector = Vector<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    i32 i = 1;
    for ( auto const& value : vector )
        verify_equal$(value, i++);
}

TEST_CASE(mutable_iterator) {
    auto vector = Vector<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for ( auto& value : vector )
        value += 1;

    i32 i = 2;
    for ( auto const& value : vector )
        verify_equal$(value, i++);
}

TEST_CASE(reverse_iterator) {
    auto vector = Vector<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    i32 i = 10;
    for ( auto const& value : vector.reverse_iter() )
        verify_equal$(value, i--);

    verify_equal$(i, 0);
}

TEST_CASE(contains_and_index_of) {
    auto const vector = Vector<i32>::construct_from_list({ 8, 34, 57, 21, 4, 3, 8, 78, 46 });

    verify$(vector.contains(21));
    verify$(vector.contains(78));
    verify$(vector.contains(3));

    VERIFY_IS_PRESENT_EQUAL(vector.index_of(21), 3);
    VERIFY_IS_PRESENT_EQUAL(vector.index_of(78), 7);
    VERIFY_IS_PRESENT_EQUAL(vector.index_of(3), 5);
    VERIFY_IS_NONE(vector.index_of(99));
}

TEST_CASE(index_if) {
    auto const vector = Vector<i32>::construct_from_list({ 8, 34, 57, 21, 4, 3, 8, 78, 46 });

    VERIFY_IS_PRESENT_EQUAL(vector.index_if([](i32 const& value) -> bool { return value == 78; }), 7);
    VERIFY_IS_PRESENT_EQUAL(vector.index_if([](i32 const& value) -> bool { return value % 2 != 0; }), 2);
}

TEST_CASE(find) {
    auto vector_mut = Vector<i32>::construct_from_list({ 0, 4, 6, 2, 3, 8, 5, 1 });

    auto value_or_none = vector_mut.find(6);
    verify$(value_or_none.is_present());

    auto& value_ref = value_or_none.unwrap();
    value_ref       = 512;

    VERIFY_IS_PRESENT_EQUAL(vector_mut.find(512), 512);

    auto const vector = Vector<i32>::construct_from_list({ 0, 4, 6, 2, 3, 8, 5, 1 });
    VERIFY_IS_PRESENT(vector.find(8));
    VERIFY_IS_NONE(vector.find(200));
}

TEST_CASE(find_if) {
    struct Pair {
        usize m_first{ 0 };
        char  m_second{ 'c' };
    };

    auto const vector = Vector<Pair>::construct_from_list({ Pair{ 4, 'a' }, Pair{ 6, 'b' }, Pair{ 8, 'c' }, Pair{ 12, 'e' } });
    VERIFY_IS_PRESENT(vector.find_if([](Pair const& o) -> bool { return o.m_first < 10 && o.m_second == 'b'; }));

    auto const vector_2 = Vector<i32>::construct_from_list({ 0, 4, 6, 2, 3, 8, 5, 1 });
    VERIFY_IS_PRESENT(vector_2.find_if([](i32 const& i) -> bool { return i == 3; }));
}

TEST_CASE(move_construction_and_copy) {
    auto vector = Vector<i32>::construct_from_list({ 1, 2, 3, 4, 5 });
    verify_false$(vector.is_empty());
    verify_equal$(vector.count(), 5);

    auto vector_2 = Vector<i32>::construct_from_other(vector);
    verify_false$(vector.is_empty());
    verify_equal$(vector.count(), 5);
    verify_false$(vector_2.is_empty());
    verify_equal$(vector_2.count(), 5);

    auto vector_3 = Cxx::move(vector);
    verify_false$(vector_3.is_empty());
    verify_equal$(vector_3.count(), 5);

    verify$(vector.is_empty());
    verify_equal$(vector.count(), 0);
}

BENCHMARK_CASE(one_hundred_thousand_append_with_reallocations) {
    struct U64 {
        u64 m_value{ 0 };

        auto operator<=>(U64 const&) const -> bool = default;
    };

    auto vector = Vector<U64>::construct_with_capacity(1);
    for ( u64 i = 0; i < 100'000; ++i )
        vector.append(U64{ i * 34 });

    verify_equal$(vector.count(), 100'000);
    verify$(vector.capacity() >= 100'000);
}

BENCHMARK_CASE(one_hundred_thousand_append_without_reallocations) {
    struct U64 {
        u64 m_value{ 0 };

        auto operator<=>(U64 const&) const -> bool = default;
    };

    auto vector = Vector<U64>::construct_with_capacity(100'000);
    for ( u64 i = 0; i < 100'000; ++i )
        vector.append(U64{ i * 34 });

    verify_equal$(vector.count(), 100'000);
    verify$(vector.capacity() >= 100'000);
}

BENCHMARK_CASE(one_hundred_thousand_append_unchecked_without_reallocations) {
    struct U64 {
        u64 m_value{ 0 };

        auto operator<=>(U64 const&) const -> bool = default;
    };

    auto vector = Vector<U64>::construct_with_capacity(100'000);
    for ( auto const i : Range{ 0uLL, 100'000uLL } )
        vector.append_unchecked(U64{ i * 34 });

    verify_equal$(vector.count(), 100'000);
    verify$(vector.capacity() >= 100'000);
}