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

#include <LibTC/Collection/List.hh>
#include <LibTC/Cxx.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(empty_list) {
    auto const empty_list = List<i32>::construct_empty();

    VERIFY(empty_list.is_empty());
    VERIFY_EQUAL(empty_list.count(), 0);

    VERIFY(0);
}

TEST_CASE(initializer_list) {
    auto const list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5 });

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(copy_list) {
    auto const list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5 });
    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);

    auto const list_copy = List<i32>::construct_from_other(list);
    VERIFY_FALSE(list_copy.is_empty());
    VERIFY_EQUAL(list_copy.count(), 5);

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(clone_list) {
    auto const list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5 });
    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);

    auto const list_clone = list.clone();
    VERIFY_FALSE(list_clone.is_empty());
    VERIFY_EQUAL(list_clone.count(), 5);

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(clear) {
    auto list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    list.clear();
    VERIFY(list.is_empty());
    VERIFY_EQUAL(list.count(), 0);
}

TEST_CASE(try_append) {
    auto list = List<i32>::construct_empty();

    VERIFY_IS_VALUE(list.try_append(1));
    VERIFY_IS_VALUE(list.try_append(2));
    VERIFY_IS_VALUE(list.try_append(3));

    i32 int4 = 4;
    VERIFY_IS_VALUE(list.try_append(int4));

    i32 int5 = 5;
    VERIFY_IS_VALUE(list.try_append(int5));

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(append) {
    struct USize {
        usize m_value{ 0 };
    };

    auto usize_list = List<USize>::construct_empty();
    VERIFY(usize_list.is_empty());

    usize_list.append(USize{ 1 });
    usize_list.append(USize{ 10 });
    usize_list.append(USize{ 100 });
    usize_list.append(USize{ 1000 });
    usize_list.append(USize{ 10'000 });

    VERIFY_FALSE(usize_list.is_empty());
    VERIFY_EQUAL(usize_list.count(), 5);
}

TEST_CASE(try_prepend) {
    auto list = List<i32>::construct_empty();

    VERIFY_IS_VALUE(list.try_prepend(1));
    VERIFY_IS_VALUE(list.try_prepend(2));
    VERIFY_IS_VALUE(list.try_prepend(3));

    i32 int4 = 4;
    VERIFY_IS_VALUE(list.try_prepend(int4));

    i32 int5 = 5;
    VERIFY_IS_VALUE(list.try_prepend(int5));

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(prepend) {
    struct USize {
        usize m_value{ 0 };
    };

    auto usize_list = List<USize>::construct_empty();
    VERIFY(usize_list.is_empty());

    usize_list.prepend(USize{ 1 });
    usize_list.prepend(USize{ 10 });
    usize_list.prepend(USize{ 100 });
    usize_list.prepend(USize{ 1000 });
    usize_list.prepend(USize{ 10'000 });

    VERIFY_FALSE(usize_list.is_empty());
    VERIFY_EQUAL(usize_list.count(), 5);
}

TEST_CASE(erase) {
    auto list = List<i32>::construct_from_list({ 9, 5, 4, 6, 3, 2 });

    list.erase_if([](auto const& v) -> bool { return v % 2; });

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 3);
}

TEST_CASE(erase_if) {
    struct Pair {
        char  m_key{ 'a' };
        usize m_value{ 0 };
    };

    auto list = List<Pair>::construct_from_list({ {}, { 'b', 1 }, { 'c', 2 }, { 'd', 3 }, { 'x', 3 }, { 'k', 3 }, { 'e', 4 }, { 'f', 5 } });

    list.erase_if([](Pair const& p) { return p.m_key != 'd' && p.m_value == 3; });

    VERIFY_EQUAL(list.count(), 6);
}

TEST_CASE(contains) {
    auto const list = List<i32>::construct_from_list({ 5, 1, 3, 78, 99, 76 });

    VERIFY(list.contains(3));
    VERIFY(list.contains(5));
    VERIFY(list.contains(99));
    VERIFY(list.contains(78));
    VERIFY_FALSE(list.contains(1000));
}

TEST_CASE(find) {
    auto list = List<i32>::construct_from_list({ 5, 1, 3, 78, 99, 76 });

    auto value_or_none = list.find(78);
    VERIFY(value_or_none.is_present());

    auto& value_ref = value_or_none.value();

    value_ref = 1000;

    VERIFY_IS_PRESENT_EQUAL(list.find(1000), 1000);
    VERIFY_IS_NONE(list.find(4));

    auto const list2 = List<i32>::construct_from_list({ 1, 2, 3, 4 });
    VERIFY_IS_PRESENT_EQUAL(list2.find(2), 2);
}

TEST_CASE(find_if) {
    struct USize {
        usize m_value{ 0 };

        [[nodiscard]] auto operator==(USize const&) const -> bool = default;
        [[nodiscard]] auto operator==(usize rhs) const -> bool {
            return m_value == rhs;
        }
    };

    auto list = List<USize>::construct_from_list({ { 100 }, { 200 }, { 300 }, { 400 } });
    VERIFY_IS_PRESENT_EQUAL(list.find_if([](USize const& o) { return o == 400; }), 400);

    auto& usize_value   = list.find_if([](USize const& o) { return o == 100; }).value();
    usize_value.m_value = 500;
    VERIFY_IS_PRESENT_EQUAL(list.find_if([](USize const& o) { return o == 500; }), 500);

    auto const list2 = List<USize>::construct_from_list({ { 100 }, { 200 }, { 500 } });
    VERIFY_IS_PRESENT_EQUAL(list2.find_if([](USize const& o) { return o == 500; }), 500);
}

TEST_CASE(first_and_last) {
    auto list = List<i32>::construct_from_list({ 1 });
    VERIFY_EQUAL(list.first(), 1);
    VERIFY_EQUAL(list.last(), 1);

    list.append(2);
    VERIFY_EQUAL(list.first(), 1);
    VERIFY_EQUAL(list.last(), 2);

    list.prepend(0);
    VERIFY_EQUAL(list.first(), 0);
    VERIFY_EQUAL(list.last(), 2);
}

TEST_CASE(const_iterator) {
    auto const list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    i32 expected = 1;
    for ( auto const& i : list )
        VERIFY_EQUAL(i, expected++);
}

TEST_CASE(mutable_iterator) {
    auto list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for ( auto& i : list )
        i += 1;

    i32 expected = 2;
    for ( auto const& i : list )
        VERIFY_EQUAL(i, expected++);
}

TEST_CASE(reverse_iterator) {
    auto const list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    i32 expected = 10;
    for ( auto const& i : list.reverse_iter() )
        VERIFY_EQUAL(i, expected--);
}

TEST_CASE(reverse_mutable_iterator) {
    auto list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for ( auto& i : list.reverse_iter() )
        i += 1;

    i32 expected = 11;
    for ( auto const& i : list.reverse_iter() )
        VERIFY_EQUAL(i, expected--);
}

TEST_CASE(assignment_operator) {
    auto list  = List<i32>::construct_empty();
    auto list2 = List<i32>::construct_from_list({ 1, 2, 3, 4, 5 });

    list = Cxx::move(list2);
    VERIFY(list2.is_empty());
    VERIFY_EQUAL(list2.count(), 0);
    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}