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

#include <LibTC/Collection/List.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Text/Format.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(base_format) {
    StringBuilder string_builder{};

    auto error_or_void = format(string_builder, "Here is a {}", nullptr);
    VERIFY_FALSE(error_or_void.is_error());

    VERIFY_EQUAL(string_builder.as_string_view(), "Here is a nullptr");
}

TEST_CASE(integral_format) {
    StringBuilder string_builder{};

    auto error_or_void = format(string_builder, "One = {}, Two = {}, Three = {}", 1, 2, 3);
    VERIFY_FALSE(error_or_void.is_error());

    VERIFY_EQUAL(string_builder.as_string_view(), "One = 1, Two = 2, Three = 3");
}

TEST_CASE(floating_format) {
    StringBuilder string_builder{};

    auto error_or_void = format(string_builder, "One = {}, Two = {}, Three = {}", 1.00, 2.009999, 3.45L);
    VERIFY_FALSE(error_or_void.is_error());

    VERIFY_EQUAL(string_builder.as_string_view(), "One = 1, Two = 2.009999, Three = 3.45");
}

TEST_CASE(list_format) {
    List list{ 1, 2, 3, 4, 5, 6 };

    StringBuilder string_builder{};

    auto error_or_void = format(string_builder, "MyList = {}", list);
    VERIFY_FALSE(error_or_void.is_error());

    VERIFY_EQUAL(string_builder.as_string_view(), "MyList = [ 1, 2, 3, 4, 5, 6 ]");
}