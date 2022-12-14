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

#include <CCLang/Alloc/StringBuilder.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(append_char) {
    auto string_builder = StringBuilder::empty();
    verify$(string_builder.is_empty());

    string_builder.append('H');
    string_builder.append('e');
    string_builder.append('l');
    string_builder.append('l');
    string_builder.append('o');

    verify_false$(string_builder.is_empty());
    verify_equal$(string_builder.as_string_view(), "Hello"sv);
    verify_equal$(string_builder.to_string(), "Hello"sv);
}

TEST_CASE(try_append_char) {
    auto string_builder = StringBuilder::empty();
    verify$(string_builder.is_empty());

    verify_is_value$(string_builder.try_append('H'));
    verify_is_value$(string_builder.try_append('e'));
    verify_is_value$(string_builder.try_append('l'));
    verify_is_value$(string_builder.try_append('l'));
    verify_is_value$(string_builder.try_append('o'));

    verify_false$(string_builder.is_empty());
    verify_equal$(string_builder.as_string_view(), "Hello"sv);
    verify_equal$(string_builder.to_string(), "Hello"sv);
}

TEST_CASE(append_string) {
    auto string_builder = StringBuilder::empty();
    verify$(string_builder.is_empty());

    string_builder.append("Hi"sv);
    string_builder.append(" my "sv);
    string_builder.append("little friend"sv);

    verify_false$(string_builder.is_empty());
    verify_equal$(string_builder.as_string_view(), "Hi my little friend"sv);
    verify_equal$(string_builder.to_string(), "Hi my little friend"sv);

    string_builder.append(""sv);
    verify_equal$(string_builder.as_string_view(), "Hi my little friend"sv);
    verify_equal$(string_builder.to_string(), "Hi my little friend"sv);
}

TEST_CASE(try_append_string) {
    auto string_builder = StringBuilder::empty();
    verify$(string_builder.is_empty());

    verify_is_value$(string_builder.try_append("Hi"sv));
    verify_is_value$(string_builder.try_append(" my "sv));
    verify_is_value$(string_builder.try_append("little"sv));
    verify_is_value$(string_builder.try_append(" "sv));
    verify_is_value$(string_builder.try_append("friend"sv));
    verify_is_value$(string_builder.try_append(" I'm happy"sv));

    verify_false$(string_builder.is_empty());
    verify_equal$(string_builder.as_string_view(), "Hi my little friend I'm happy"sv);
    verify_equal$(string_builder.to_string(), "Hi my little friend I'm happy"sv);
}