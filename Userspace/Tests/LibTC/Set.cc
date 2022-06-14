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
    Set<usize> set{};

    VERIFY(set.is_empty());
    VERIFY_EQUAL(set.count(), 0);
}

TEST_CASE(initializer_list) {
    Set<StringView> set{ "Hey", "I'm", "abc" };

    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);
}

TEST_CASE(insert) {
    Set<usize> set{};

    VERIFY_IS_VALUE_EQUAL(set.try_insert(10), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(100), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(1000), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(100), InsertResult::ReplacedExisting);
    VERIFY_IS_VALUE_EQUAL(set.try_insert(1000, OnExistingEntry::Keep), InsertResult::KeptExisting);

    VERIFY_FALSE(set.is_empty());
    VERIFY_EQUAL(set.count(), 3);
}

TEST_CASE(iterate) {
    Set<StringView> set{ "One", "Two", "Three" };

    int iter_count = 0;
    for ( auto const& value : set ) {
        VERIFY_FALSE(value.is_null_or_empty());

        ++iter_count;
    }
    VERIFY_EQUAL(iter_count, 3);

    OrderedSet<usize> ordered_set{ 10, 9, 8, 7 };

    iter_count           = 0;
    usize expected_value = 10;
    for ( auto const& value : ordered_set ) {
        VERIFY_EQUAL(value, expected_value--);

        ++iter_count;
    }
    VERIFY_EQUAL(iter_count, 4);
}

TEST_CASE(find) {
    Set<String> set{ "aa", "bb", "cc", "dd" };

    VERIFY_IS_PRESENT_EQUAL(set.find("aa"), "aa");
    VERIFY_IS_PRESENT_EQUAL(set.find("cc"), "cc");
    VERIFY_IS_PRESENT_EQUAL(set.find("bb"), "bb");
    VERIFY_IS_NONE(set.find("xx"));
}

TEST_CASE(remove) {
    OrderedSet<StringView> set{ "Hi", "I'm", "Marco" };

    VERIFY(set.remove("I'm"));
    VERIFY_EQUAL(set.count(), 2);

    VERIFY(set.remove("Hi"));
    VERIFY_EQUAL(set.count(), 1);

    VERIFY_IS_VALUE_EQUAL(set.try_insert("Hello"), InsertResult::InsertedNew);

    VERIFY_FALSE(set.remove("Eee"));
    VERIFY_EQUAL(set.count(), 2);
}

TEST_CASE(remove_all_matching) {
    Set<StringView> set{ "Aka", "Biko", "Ako", "Biko" };

    VERIFY_EQUAL(set.remove_all_matching([](auto const& value) { return value.starts_with("A"); }), 2);
}

TEST_CASE(ensure_capacity) {
    Set<usize> set{};

    VERIFY_EQUAL(set.capacity(), 0);

    VERIFY_IS_VALUE(set.try_ensure_capacity(10'000));
    VERIFY_GREATER_EQUAL(set.capacity(), 10'000);
    VERIFY_EQUAL(set.count(), 0);
    VERIFY(set.is_empty());
}