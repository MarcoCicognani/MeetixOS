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

#include <CCLang/Alloc/Set.hh>
#include <CCLang/Alloc/String.hh>
#include <CCLang/Lang/StringView.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(default_construction) {
    auto const set = Set<i32>::empty();

    verify$(set.is_empty());
    verify_equal$(set.count(), 0);
}

TEST_CASE(initializer_list) {
    auto const set = Set<StringView>::from_list({ "Hey"sv, "I'm"sv, "abc"sv });

    verify_false$(set.is_empty());
    verify_equal$(set.count(), 3);
}

TEST_CASE(copy_other) {
    auto const set = Set<StringView>::from_list({ "Hey"sv, "I'm"sv, "abc"sv });
    verify_false$(set.is_empty());
    verify_equal$(set.count(), 3);

    auto const other_set = Set<StringView>::from_other(set);
    verify_false$(other_set.is_empty());
    verify_equal$(other_set.count(), 3);
}

TEST_CASE(clone_other) {
    auto const set = Set<StringView>::from_list({ "Hey"sv, "I'm"sv, "abc"sv });
    verify_false$(set.is_empty());
    verify_equal$(set.count(), 3);

    auto const other_set = set.clone();
    verify_false$(other_set.is_empty());
    verify_equal$(other_set.count(), 3);

    verify_false$(set.is_empty());
    verify_equal$(set.count(), 3);
}

TEST_CASE(insert) {
    auto set = Set<i32>::with_capacity(3);

    VERIFY_IS_VALUE_EQUAL(set.try_insert(10), SetInsertResult::New);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(100), SetInsertResult::New);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(1000), SetInsertResult::New);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(100), SetInsertResult::Replaced);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(1000, SetReplaceExisting::No), SetInsertResult::Kept);

    verify_false$(set.is_empty());
    verify_equal$(set.count(), 3);
}

TEST_CASE(iterate) {
    auto const set = Set<StringView>::from_list({ "One"sv, "Two"sv, "Three"sv });

    i32 i = 0;
    for ( auto const& value : set ) {
        verify_false$(value.is_null_or_empty());

        ++i;
    }
    verify_equal$(i, 3);

    auto const ordered_set = OrderedSet<i32>::from_list({ 1, 2, 3, 4 });

    i            = 0;
    i32 expected = 1;
    for ( auto const& value : ordered_set ) {
        verify_equal$(value, expected++);

        ++i;
    }
    verify_equal$(i, 4);
}

TEST_CASE(ordered_set_reverse_iterator) {
    auto const ordered_set = OrderedSet<i32>::from_list({ 1, 2, 3, 4, 5 });

    i32 expected = 5;
    for ( auto const i : ordered_set.reverse_iter() )
        verify_equal$(i, expected--);
}

TEST_CASE(find) {
    auto const set = Set<StringView>::from_list({ "aa"sv, "bb"sv, "cc"sv, "dd"sv });

    VERIFY_IS_PRESENT_EQUAL(set.find("aa"sv), "aa"sv);
    VERIFY_IS_PRESENT_EQUAL(set.find("cc"sv), "cc"sv);
    VERIFY_IS_PRESENT_EQUAL(set.find("bb"sv), "bb"sv);
    VERIFY_IS_NONE(set.find("xx"sv));
}

TEST_CASE(remove) {
    auto ordered_set = OrderedSet<StringView>::from_list({ "Hi"sv, "I'm"sv, "Marco"sv });

    verify$(ordered_set.remove("I'm"sv));
    verify_equal$(ordered_set.count(), 2);

    verify$(ordered_set.remove("Hi"sv));
    verify_equal$(ordered_set.count(), 1);

    VERIFY_IS_VALUE_EQUAL(ordered_set.try_insert("Hello"sv), SetInsertResult::New);

    verify_false$(ordered_set.remove("Eee"sv));
    verify_equal$(ordered_set.count(), 2);
}

TEST_CASE(remove_all_matching) {
    auto set = Set<StringView>::from_list({ "Aka"sv, "Biko"sv, "Ako"sv, "Biko"sv });

    verify_equal$(set.remove_all_matching([](auto const& value) { return value.starts_with("A"sv); }), 2);
}

TEST_CASE(ensure_capacity) {
    auto set = Set<i32>::empty();
    verify_equal$(set.capacity(), 0);

    VERIFY_IS_VALUE(set.try_ensure_capacity(10'000));
    verify_greater_equal$(set.capacity(), 10'000);
    verify_equal$(set.count(), 0);
    verify$(set.is_empty());
}