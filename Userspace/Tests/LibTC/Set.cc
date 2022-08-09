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

#include <LibTC/Collection/Set.hh>
#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(default_construction) {
    auto const set = Set<i32>::construct_empty();

    VERIFY(set.is_empty());
    VERIFY_EQUAL(set.count(), 0);
}

TEST_CASE(initializer_list) {
    auto const set = Set<StringView>::construct_from_list({ "Hey"sv, "I'm"sv, "abc"sv });

    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);
}

TEST_CASE(copy_other) {
    auto const set = Set<StringView>::construct_from_list({ "Hey"sv, "I'm"sv, "abc"sv });
    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);

    auto const other_set = Set<StringView>::construct_from_other(set);
    VERIFY_FALSE(other_set.is_empty());
    VERIFY_EQUAL(other_set.count(), 3);
}

TEST_CASE(clone_other) {
    auto const set = Set<StringView>::construct_from_list({ "Hey"sv, "I'm"sv, "abc"sv });
    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);

    auto const other_set = set.clone();
    VERIFY_FALSE(other_set.is_empty());
    VERIFY_EQUAL(other_set.count(), 3);

    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);
}

TEST_CASE(insert) {
    auto set = Set<i32>::construct_with_capacity(3);

    VERIFY_IS_VALUE_EQUAL(set.try_insert(10), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(100), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(1000), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(100), InsertResult::ReplacedExisting);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(1000, OnExistingEntry::Keep), InsertResult::KeptExisting);

    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);
}

TEST_CASE(iterate) {
    auto const set = Set<StringView>::construct_from_list({ "One"sv, "Two"sv, "Three"sv });

    i32 i = 0;
    for ( auto const& value : set ) {
        VERIFY_FALSE(value.is_null_or_empty());

        ++i;
    }
    VERIFY_EQUAL(i, 3);

    auto const ordered_set = OrderedSet<i32>::construct_from_list({ 1, 2, 3, 4 });

    i            = 0;
    i32 expected = 10;
    for ( auto const& value : ordered_set ) {
        VERIFY_EQUAL(value, expected--);

        ++i;
    }
    VERIFY_EQUAL(i, 4);
}

TEST_CASE(ordered_set_reverse_iterator) {
    auto const ordered_set = OrderedSet<i32>::construct_from_list({ 1, 2, 3, 4, 5 });

    i32 expected = 5;
    for ( auto const i : ordered_set.reverse_iter() )
        VERIFY_EQUAL(i, expected--);
}

TEST_CASE(find) {
    auto const set = Set<StringView>::construct_from_list({ "aa"sv, "bb"sv, "cc"sv, "dd"sv });

    VERIFY_IS_PRESENT_EQUAL(set.find("aa"sv), "aa"sv);
    VERIFY_IS_PRESENT_EQUAL(set.find("cc"sv), "cc"sv);
    VERIFY_IS_PRESENT_EQUAL(set.find("bb"sv), "bb"sv);
    VERIFY_IS_NONE(set.find("xx"sv));
}

TEST_CASE(remove) {
    auto ordered_set = OrderedSet<StringView>::construct_from_list({ "Hi"sv, "I'm"sv, "Marco"sv });

    VERIFY(ordered_set.remove("I'm"sv));
    VERIFY_EQUAL(ordered_set.count(), 2);

    VERIFY(ordered_set.remove("Hi"sv));
    VERIFY_EQUAL(ordered_set.count(), 1);

    VERIFY_IS_VALUE_EQUAL(ordered_set.try_insert("Hello"sv), InsertResult::InsertedNew);

    VERIFY_FALSE(ordered_set.remove("Eee"sv));
    VERIFY_EQUAL(ordered_set.count(), 2);
}

TEST_CASE(remove_all_matching) {
    auto set = Set<StringView>::construct_from_list({ "Aka"sv, "Biko"sv, "Ako"sv, "Biko"sv });

    VERIFY_EQUAL(set.remove_all_matching([](auto const& value) { return value.starts_with("A"sv); }), 2);
}

TEST_CASE(ensure_capacity) {
    auto set = Set<i32>::construct_empty();
    VERIFY_EQUAL(set.capacity(), 0);

    VERIFY_IS_VALUE(set.try_ensure_capacity(10'000));
    VERIFY_GREATER_EQUAL(set.capacity(), 10'000);
    VERIFY_EQUAL(set.count(), 0);
    VERIFY(set.is_empty());
}