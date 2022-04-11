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

#include <LibTC/Collection/StringBuilder.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(append_char) {
    StringBuilder string_builder{};
    VERIFY(string_builder.is_empty());

    string_builder.append('H');
    string_builder.append('e');
    string_builder.append('l');
    string_builder.append('l');
    string_builder.append('o');

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hello"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hello"sv);
}

TEST_CASE(try_append_char) {
    StringBuilder string_builder{};
    VERIFY(string_builder.is_empty());

    auto error_or_void = string_builder.try_append('H');
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append('e');
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append('l');
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append('l');
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append('o');
    VERIFY(error_or_void.is_value());

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hello"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hello"sv);
}

TEST_CASE(append_string) {
    StringBuilder string_builder{};
    VERIFY(string_builder.is_empty());

    string_builder.append("Hi");
    string_builder.append(" my ");
    string_builder.append("little friend");

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hi my little friend"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hi my little friend"sv);

    string_builder.append("");
    VERIFY_EQUAL(string_builder.as_string_view(), "Hi my little friend"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hi my little friend"sv);
}

TEST_CASE(try_append_string) {
    StringBuilder string_builder{};
    VERIFY(string_builder.is_empty());

    auto error_or_void = string_builder.try_append("Hi");
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append(" my ");
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append("little");
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append(" ");
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append("friend");
    VERIFY(error_or_void.is_value());

    error_or_void = string_builder.try_append(" I'm happy");
    VERIFY(error_or_void.is_value());

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hi my little friend I'm happy"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hi my little friend I'm happy"sv);
}