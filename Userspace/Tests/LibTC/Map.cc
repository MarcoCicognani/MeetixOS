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
#include <LibTC/IntTypes.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(default_construction) {
    Map<usize, usize> map{};

    VERIFY(map.is_empty());
    VERIFY_EQUAL(map.count(), 0);
}

TEST_CASE(initializer_list) {
    Map<usize, StringView> map{ { 10uL, "Hello" }, { 100uL, "World" }, { 1000uL, "Guys" } };

    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 3);
}

TEST_CASE(insert) {
    Map<String, String> map{};

    VERIFY_IS_VALUE_EQUAL(map.try_insert("a", "xxx"), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("b", "yyy"), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("c", "zzz"), InsertResult::InsertedNew);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("b", "000"), InsertResult::ReplacedExisting);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("c", "aaa", OnExistingEntry::Keep), InsertResult::KeptExisting);

    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 3);
}

TEST_CASE(iterate) {
    Map<StringView, usize> map{ { "One", 1 }, { "Two", 2 }, { "Three", 3 } };

    int iter_count = 0;
    for ( auto const& pair : map ) {
        VERIFY_FALSE(pair.key().is_empty());
        VERIFY_NOT_EQUAL(pair.value(), 0);

        ++iter_count;
    }
    VERIFY_EQUAL(iter_count, 3);

    OrderedMap<usize, usize> ordered_map{ { 10, 1 }, { 9, 2 }, { 8, 3 } };

    iter_count           = 0;
    usize expected_key   = 10;
    usize expected_value = 1;
    for ( auto const& pair : ordered_map ) {
        VERIFY_EQUAL(pair.key(), expected_key--);
        VERIFY_EQUAL(pair.value(), expected_value++);

        ++iter_count;
    }
    VERIFY_EQUAL(iter_count, 3);
}

TEST_CASE(at) {
    Map<StringView, StringView> map{ { "One", "1" }, { "Two", "2" }, { "Three", "3" } };

    VERIFY_IS_PRESENT_EQUAL(map["One"], "1");
    VERIFY_IS_PRESENT_EQUAL(map.at("Three"), "3");
    VERIFY_IS_PRESENT_EQUAL(map["Two"], "2");
    VERIFY_IS_NONE(map["Four"]);
    VERIFY_IS_NONE(map.at("Five"));

    auto value_or_none    = map.at("One");
    value_or_none.value() = StringView{ "15" };

    VERIFY_IS_PRESENT_EQUAL(map["One"], "15");
}

TEST_CASE(has_key_has_value) {
    Map<usize, String> map{ { 0xabc, "Hi" }, { 0xfab, "My" }, { 0xfff, "Friends" } };

    VERIFY(map.has_key(0xabc));
    VERIFY(map.has_key(0xfff));
    VERIFY_FALSE(map.has_key(0x45f));

    VERIFY(map.has_value("Hi"));
    VERIFY(map.has_value("My"));
    VERIFY_FALSE(map.has_value("Teacher"));
}

TEST_CASE(remove) {
    Map<StringView, StringView> map{ { "One", "Bla" }, { "Two", "Ble" }, { "Three", "Blu" } };

    VERIFY(map.remove("One"));
    VERIFY_EQUAL(map.count(), 2);

    VERIFY(map.remove("Three"));
    VERIFY_EQUAL(map.count(), 1);

    VERIFY_IS_VALUE_EQUAL(map.try_insert("Hundred", "Black"), InsertResult::InsertedNew);

    VERIFY_FALSE(map.remove("Five"));
    VERIFY_EQUAL(map.count(), 2);
}

TEST_CASE(remove_all_matching) {
    Map<StringView, StringView> map{ { "One", "Bla" }, { "Two", "Tru" }, { "Three", "Blu" } };

    VERIFY_EQUAL(map.remove_all_matching([](auto const&, auto const& value) { return value.starts_with("B"); }), 2);
}

TEST_CASE(ensure_capacity) {
    Map<usize, usize> map{};

    VERIFY_EQUAL(map.capacity(), 0);

    VERIFY_IS_VALUE(map.try_ensure_capacity(10'000));
    VERIFY_GREATER_EQUAL(map.capacity(), 10'000);
    VERIFY_EQUAL(map.count(), 0);
    VERIFY(map.is_empty());
}