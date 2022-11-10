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

#include <LibLS/Alloc/Map.hh>
#include <LibLS/Alloc/String.hh>
#include <LibLS/Lang/StringView.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(default_construction) {
    auto const map = Map<i32, u64>::construct_empty();

    verify$(map.is_empty());
    verify_equal$(map.count(), 0);
}

TEST_CASE(initializer_list) {
    auto const map = Map<i32, StringView>::construct_from_list({ { 10, "Hello"sv }, { 100, "World"sv }, { 1000, "Guys"sv } });

    verify_false$(map.is_empty());
    verify_equal$(map.count(), 3);
}

TEST_CASE(construct_from_other) {
    auto const map = Map<StringView, NonNullRef<i32>>::construct_from_list(
        { { "One"sv, NonNullRef<i32>::construct_from_emplace(10) }, { "Two"sv, NonNullRef<i32>::construct_from_emplace(20) } });

    auto const map_2 = Map<StringView, NonNullRef<i32>>::construct_from_other(map);

    verify_false$(map.is_empty());
    verify_equal$(map.count(), 2);
}

TEST_CASE(insert) {
    auto map = Map<StringView, StringView>::construct_empty();

    VERIFY_IS_VALUE_EQUAL(map.try_insert("a"sv, "xxx"sv), MapInsertResult::New);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("b"sv, "yyy"sv), MapInsertResult::New);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("c"sv, "zzz"sv), MapInsertResult::New);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("b"sv, "000"sv), MapInsertResult::Replaced);
    VERIFY_IS_VALUE_EQUAL(map.try_insert("c"sv, "aaa"sv, MapReplaceExisting::No), MapInsertResult::Kept);

    verify_false$(map.is_empty());
    verify_equal$(map.count(), 3);
}

TEST_CASE(iterate) {
    auto const map = Map<StringView, i32>::construct_from_list({ { "One"sv, 1 }, { "Two"sv, 2 }, { "Three"sv, 3 } });

    i32 i = 0;
    for ( auto const& pair : map ) {
        verify_false$(pair.m_key.is_null_or_empty());
        verify_not_equal$(pair.m_value, 0);

        ++i;
    }
    verify_equal$(i, 3);

    auto const ordered_map = OrderedMap<i32, i32>::construct_from_list({ { 10, 1 }, { 9, 2 }, { 8, 3 } });

    i                  = 0;
    i32 expected_key   = 10;
    i32 expected_value = 1;
    for ( auto const& pair : ordered_map ) {
        verify_equal$(pair.m_key, expected_key--);
        verify_equal$(pair.m_value, expected_value++);

        ++i;
    }
    verify_equal$(i, 3);
}

TEST_CASE(ordered_reverse_iterate) {
    auto const ordered_map = OrderedMap<i32, i32>::construct_from_list({ { 1, 3 }, { 2, 2 }, { 3, 1 } });

    i32 expected_key   = 3;
    i32 expected_value = 1;
    for ( auto const& pair : ordered_map.reverse_iter() ) {
        verify_equal$(pair.m_key, expected_key--);
        verify_equal$(pair.m_value, expected_value++);
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

    verify$(map.has_key(0xabc));
    verify$(map.has_key(0xfff));
    verify_false$(map.has_key(0x45f));

    verify$(map.has_value("Hi"sv));
    verify$(map.has_value("My"sv));
    verify_false$(map.has_value("Teacher"sv));
}

TEST_CASE(remove) {
    auto map = Map<StringView, StringView>::construct_from_list({ { "One"sv, "Bla"sv }, { "Two"sv, "Ble"sv }, { "Three"sv, "Blu"sv } });

    verify$(map.remove("One"sv));
    verify_equal$(map.count(), 2);

    verify$(map.remove("Three"sv));
    verify_equal$(map.count(), 1);

    VERIFY_IS_VALUE_EQUAL(map.try_insert("Hundred"sv, "Black"sv), MapInsertResult::New);

    verify_false$(map.remove("Five"sv));
    verify_equal$(map.count(), 2);
}

TEST_CASE(remove_all_matching) {
    auto map = Map<StringView, StringView>::construct_from_list({ { "One"sv, "Bla"sv }, { "Two"sv, "Tru"sv }, { "Three"sv, "Blu"sv } });

    verify_equal$(map.remove_all_matching([](auto const& pair) { return pair.m_value.starts_with("B"sv); }), 2);
}

TEST_CASE(ensure_capacity) {
    auto map = Map<i32, i32>::construct_empty();
    verify_equal$(map.capacity(), 0);

    VERIFY_IS_VALUE(map.try_ensure_capacity(10'000));
    verify_greater_equal$(map.capacity(), 10'000);
    verify_equal$(map.count(), 0);
    verify$(map.is_empty());
}