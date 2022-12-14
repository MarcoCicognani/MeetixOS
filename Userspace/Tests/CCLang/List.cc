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

#include <CCLang/Alloc/List.hh>
#include <CCLang/Lang/Cxx.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(empty_list) {
    auto const empty_list = List<i32>::empty();

    verify$(empty_list.is_empty());
    verify_equal$(empty_list.count(), 0);
}

TEST_CASE(initializer_list) {
    auto const list = List<i32>::from_list({ 1, 2, 3, 4, 5 });

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}

TEST_CASE(copy_list) {
    auto const list = List<i32>::from_list({ 1, 2, 3, 4, 5 });
    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);

    auto const list_copy = List<i32>::from_other(list);
    verify_false$(list_copy.is_empty());
    verify_equal$(list_copy.count(), 5);

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}

TEST_CASE(clone_list) {
    auto const list = List<i32>::from_list({ 1, 2, 3, 4, 5 });
    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);

    auto const list_clone = list.clone();
    verify_false$(list_clone.is_empty());
    verify_equal$(list_clone.count(), 5);

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}

TEST_CASE(clear) {
    auto list = List<i32>::from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    list.clear();
    verify$(list.is_empty());
    verify_equal$(list.count(), 0);
}

TEST_CASE(try_append) {
    auto list = List<i32>::empty();

    verify_is_value$(list.try_append(1));
    verify_is_value$(list.try_append(2));
    verify_is_value$(list.try_append(3));

    i32 const int4 = 4;
    verify_is_value$(list.try_append(int4));

    i32 const int5 = 5;
    verify_is_value$(list.try_append(int5));

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}

TEST_CASE(append) {
    struct Wrapper {
        usize m_value{ 0 };
    };

    auto list = List<Wrapper>::empty();
    verify$(list.is_empty());

    list.append(Wrapper{ 1 });
    list.append(Wrapper{ 10 });
    list.append(Wrapper{ 100 });
    list.append(Wrapper{ 1000 });
    list.append(Wrapper{ 10'000 });

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}

TEST_CASE(try_prepend) {
    auto list = List<i32>::empty();

    verify_is_value$(list.try_prepend(1));
    verify_is_value$(list.try_prepend(2));
    verify_is_value$(list.try_prepend(3));

    i32 const int4 = 4;
    verify_is_value$(list.try_prepend(int4));

    i32 const int5 = 5;
    verify_is_value$(list.try_prepend(int5));

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}

TEST_CASE(prepend) {
    struct Wrapper {
        usize m_value{ 0 };
    };

    auto usize_list = List<Wrapper>::empty();
    verify$(usize_list.is_empty());

    usize_list.prepend(Wrapper{ 1 });
    usize_list.prepend(Wrapper{ 10 });
    usize_list.prepend(Wrapper{ 100 });
    usize_list.prepend(Wrapper{ 1000 });
    usize_list.prepend(Wrapper{ 10'000 });

    verify_false$(usize_list.is_empty());
    verify_equal$(usize_list.count(), 5);
}

TEST_CASE(erase) {
    auto list = List<i32>::from_list({ 9, 5, 4, 6, 3, 2 });

    list.erase_if([](i32 const& v) -> bool { return v % 2 == 0; });

    verify_false$(list.is_empty());
    verify_equal$(list.count(), 3);
}

TEST_CASE(erase_if) {
    struct Pair {
        char  m_key   = 'a';
        usize m_value = 0;
    };

    auto list = List<Pair>::from_list({ {}, { 'b', 1 }, { 'c', 2 }, { 'd', 3 }, { 'x', 3 }, { 'k', 3 }, { 'e', 4 }, { 'f', 5 } });

    list.erase_if([](Pair const& p) { return p.m_key != 'd' && p.m_value == 3; });

    verify_equal$(list.count(), 6);
}

TEST_CASE(contains) {
    auto const list = List<i32>::from_list({ 5, 1, 3, 78, 99, 76 });

    verify$(list.contains(3));
    verify$(list.contains(5));
    verify$(list.contains(99));
    verify$(list.contains(78));
    verify_false$(list.contains(1000));
}

TEST_CASE(find) {
    auto list = List<i32>::from_list({ 5, 1, 3, 78, 99, 76 });

    auto value_or_none = list.find(78);
    verify$(value_or_none.is_present());

    auto& ref = value_or_none.unwrap();
    ref       = 1000;

    verify_is_present_equal$(list.find(1000), 1000);
    verify_is_none$(list.find(4));

    auto const list2 = List<i32>::from_list({ 1, 2, 3, 4 });
    verify_is_present_equal$(list2.find(2), 2);
}

TEST_CASE(find_if) {
    struct Wrapper {
        usize m_value = 0;

        auto operator==(Wrapper const&) const -> bool = default;
        auto operator==(usize rhs) const -> bool {
            return m_value.equal(rhs);
        }
    };

    auto list = List<Wrapper>::from_list({ { 100 }, { 200 }, { 300 }, { 400 } });
    verify_is_present_equal$(list.find_if([](Wrapper const& o) { return o == 400; }), 400);

    auto& usize_value   = list.find_if([](Wrapper const& o) { return o == 100; }).unwrap();
    usize_value.m_value = 500;
    verify_is_present_equal$(list.find_if([](Wrapper const& o) { return o == 500; }), 500);

    auto const list2 = List<Wrapper>::from_list({ { 100 }, { 200 }, { 500 } });
    verify_is_present_equal$(list2.find_if([](Wrapper const& o) { return o == 500; }), 500);
}

TEST_CASE(first_and_last) {
    auto list = List<i32>::from_list({ 1 });
    verify_equal$(list.first(), 1);
    verify_equal$(list.last(), 1);

    list.append(2);
    verify_equal$(list.first(), 1);
    verify_equal$(list.last(), 2);

    list.prepend(0);
    verify_equal$(list.first(), 0);
    verify_equal$(list.last(), 2);
}

TEST_CASE(const_iterator) {
    auto const list = List<i32>::from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    i32 expected = 1;
    for ( auto const& i : list ) {
        verify_equal$(i, expected++);
    }
}

TEST_CASE(mutable_iterator) {
    auto list = List<i32>::from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for ( auto& i : list ) {
        i += 1;
    }

    i32 expected = 2;
    for ( auto const& i : list ) {
        verify_equal$(i, expected++);
    }
}

TEST_CASE(reverse_iterator) {
    auto const list = List<i32>::from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    i32 expected = 10;
    for ( auto const& i : list.reverse_iter() ) {
        verify_equal$(i, expected--);
    }
}

TEST_CASE(reverse_mutable_iterator) {
    auto list = List<i32>::from_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for ( auto& i : list.reverse_iter() ) {
        i += 1;
    }

    i32 expected = 11;
    for ( auto const& i : list.reverse_iter() ) {
        verify_equal$(i, expected--);
    }
}

TEST_CASE(assignment_operator) {
    auto list  = List<i32>::empty();
    auto list2 = List<i32>::from_list({ 1, 2, 3, 4, 5 });

    list = Cxx::move(list2);
    verify$(list2.is_empty());
    verify_equal$(list2.count(), 0);
    verify_false$(list.is_empty());
    verify_equal$(list.count(), 5);
}