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
    auto string_builder = StringBuilder::construct_empty();
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
    auto string_builder = StringBuilder::construct_empty();
    VERIFY(string_builder.is_empty());

    VERIFY_IS_VALUE(string_builder.try_append('H'));
    VERIFY_IS_VALUE(string_builder.try_append('e'));
    VERIFY_IS_VALUE(string_builder.try_append('l'));
    VERIFY_IS_VALUE(string_builder.try_append('l'));
    VERIFY_IS_VALUE(string_builder.try_append('o'));

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hello"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hello"sv);
}

TEST_CASE(append_string) {
    auto string_builder = StringBuilder::construct_empty();
    VERIFY(string_builder.is_empty());

    string_builder.append("Hi"sv);
    string_builder.append(" my "sv);
    string_builder.append("little friend"sv);

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hi my little friend"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hi my little friend"sv);

    string_builder.append(""sv);
    VERIFY_EQUAL(string_builder.as_string_view(), "Hi my little friend"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hi my little friend"sv);
}

TEST_CASE(try_append_string) {
    auto string_builder = StringBuilder::construct_empty();
    VERIFY(string_builder.is_empty());

    VERIFY_IS_VALUE(string_builder.try_append("Hi"sv));
    VERIFY_IS_VALUE(string_builder.try_append(" my "sv));
    VERIFY_IS_VALUE(string_builder.try_append("little"sv));
    VERIFY_IS_VALUE(string_builder.try_append(" "sv));
    VERIFY_IS_VALUE(string_builder.try_append("friend"sv));
    VERIFY_IS_VALUE(string_builder.try_append(" I'm happy"sv));

    VERIFY_FALSE(string_builder.is_empty());
    VERIFY_EQUAL(string_builder.as_string_view(), "Hi my little friend I'm happy"sv);
    VERIFY_EQUAL(string_builder.to_string(), "Hi my little friend I'm happy"sv);
}