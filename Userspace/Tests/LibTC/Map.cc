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

#include <LibTC/Collection/Map.hh>
#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(default_construction) {
    auto const map = Map<i32, u64>::construct_empty();

    VERIFY(map.is_empty());
    VERIFY_EQUAL(map.count(), 0);
}

TEST_CASE(initializer_list) {
    auto const map = Map<i32, StringView>::construct_from_list({ { 10, "Hello"sv }, { 100, "World"sv }, { 1000, "Guys"sv } });

    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 3);
}

TEST_CASE(construct_from_other) {
    auto const map = Map<StringView, NonNullRef<i32>>::construct_from_list(
        { { "One"sv, NonNullRef<i32>::construct_from_args(10) }, { "Two"sv, NonNullRef<i32>::construct_from_args(20) } });

    auto const map_2 = Map<StringView, NonNullRef<i32>>::construct_from_other(map);

    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 2);
}

TEST_CASE(insert) {
    auto map = Map<StringView, StringView>::construct_empty();

    VERIFY_IS_VALUE_EQUAL(map.try_insert("a"sv, "xxx"sv), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("b"sv, "yyy"sv), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("c"sv, "zzz"sv), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("b"sv, "000"sv), InsertResult::ReplacedExisting);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("c"sv, "aaa"sv, OnExistingEntry::Keep), InsertResult::KeptExisting);

    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 3);
}

TEST_CASE(iterate) {
    auto const map = Map<StringView, i32>::construct_from_list({ { "One"sv, 1 }, { "Two"sv, 2 }, { "Three"sv, 3 } });

    i32 i = 0;
    for ( auto const& pair : map ) {
        VERIFY_FALSE(pair.m_key.is_empty());
        VERIFY_NOT_EQUAL(pair.m_value, 0);

        ++i;
    }
    VERIFY_EQUAL(i, 3);

    auto const ordered_map = OrderedMap<i32, i32>::construct_from_list({ { 10, 1 }, { 9, 2 }, { 8, 3 } });

    i                  = 0;
    i32 expected_key   = 10;
    i32 expected_value = 1;
    for ( auto const& pair : ordered_map ) {
        VERIFY_EQUAL(pair.m_key, expected_key--);
        VERIFY_EQUAL(pair.m_value, expected_value++);

        ++i;
    }
    VERIFY_EQUAL(i, 3);
}

TEST_CASE(ordered_reverse_iterate) {
    auto const ordered_map = OrderedMap<i32, i32>::construct_from_list({ { 1, 3 }, { 2, 2 }, { 3, 1 } });

    i32 expected_key   = 1;
    i32 expected_value = 3;
    for ( auto const& pair : ordered_map.reverse_iter() ) {
        VERIFY_EQUAL(pair.m_key, expected_key--);
        VERIFY_EQUAL(pair.m_value, expected_value--);
    }
}

TEST_CASE(at) {
    auto map = Map<StringView, StringView>::construct_from_list({ { "One"sv, "1"sv }, { "Two"sv, "2"sv }, { "Three"sv, "3"sv } });

    VERIFY_IS_PRESENT_EQUAL(map["One"sv], "1"sv);
    VERIFY_IS_PRESENT_EQUAL(map.at("Three"sv), "3"sv);
    VERIFY_IS_PRESENT_EQUAL(map["Two"sv], "2"sv);
    VERIFY_IS_NONE(map["Four"sv]);
    VERIFY_IS_NONE(map.at("Five"sv));

    auto value_or_none    = map.at("One"sv);
    value_or_none.value() = "15"sv;

    VERIFY_IS_PRESENT_EQUAL(map["One"sv], "15"sv);
}

TEST_CASE(has_key_has_value) {
    auto const map = Map<i32, StringView>::construct_from_list({ { 0xabc, "Hi"sv }, { 0xfab, "My"sv }, { 0xfff, "Friends"sv } });

    VERIFY(map.has_key(0xabc));
    VERIFY(map.has_key(0xfff));
    VERIFY_FALSE(map.has_key(0x45f));

    VERIFY(map.has_value("Hi"sv));
    VERIFY(map.has_value("My"sv));
    VERIFY_FALSE(map.has_value("Teacher"sv));
}

TEST_CASE(remove) {
    auto map = Map<StringView, StringView>::construct_from_list({ { "One"sv, "Bla"sv }, { "Two"sv, "Ble"sv }, { "Three"sv, "Blu"sv } });

    VERIFY(map.remove("One"sv));
    VERIFY_EQUAL(map.count(), 2);

    VERIFY(map.remove("Three"sv));
    VERIFY_EQUAL(map.count(), 1);

    VERIFY_IS_VALUE_EQUAL(map.try_insert("Hundred"sv, "Black"sv), InsertResult::InsertedNew);

    VERIFY_FALSE(map.remove("Five"sv));
    VERIFY_EQUAL(map.count(), 2);
}

TEST_CASE(remove_all_matching) {
    auto map = Map<StringView, StringView>::construct_from_list({ { "One"sv, "Bla"sv }, { "Two"sv, "Tru"sv }, { "Three"sv, "Blu"sv } });

    VERIFY_EQUAL(map.remove_all_matching([](auto const& pair) { return pair.m_value.starts_with("B"sv); }), 2);
}

TEST_CASE(ensure_capacity) {
    auto map = Map<i32, i32>::construct_empty();
    VERIFY_EQUAL(map.capacity(), 0);

    VERIFY_IS_VALUE(map.try_ensure_capacity(10'000));
    VERIFY_GREATER_EQUAL(map.capacity(), 10'000);
    VERIFY_EQUAL(map.count(), 0);
    VERIFY(map.is_empty());
}