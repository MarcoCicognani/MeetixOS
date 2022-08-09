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

#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Cxx.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(construction) {
    StringView empty_string_view{};
    VERIFY(empty_string_view.is_empty());
    VERIFY(empty_string_view.is_null());
    VERIFY(empty_string_view.is_null_or_empty());
    VERIFY_EQUAL(empty_string_view, ""sv);

    auto const string                  = String::construct_from_view("Testing LibTC Strings"sv);
    auto const from_string_string_view = string.as_string_view();
    VERIFY_FALSE(from_string_string_view.is_null_or_empty());
    VERIFY_EQUAL(from_string_string_view.len(), 21);
    VERIFY_EQUAL(from_string_string_view.len(), string.len());
    VERIFY_EQUAL(from_string_string_view, "Testing LibTC Strings"sv);
    VERIFY_EQUAL(from_string_string_view, String::construct_from_view("Testing LibTC Strings"sv));
}

TEST_CASE(assignment_operators) {
    StringView string_view{};
    VERIFY(string_view.is_null_or_empty());

    string_view = "Hello World"sv;
    VERIFY_EQUAL(string_view.len(), 11);
    VERIFY_EQUAL(string_view, "Hello World"sv);

    StringView string_view2{};
    string_view2 = string_view;
    VERIFY_FALSE(string_view.is_null_or_empty());
    VERIFY_FALSE(string_view2.is_null_or_empty());
    VERIFY_EQUAL(string_view, "Hello World"sv);
    VERIFY_EQUAL(string_view2, "Hello World"sv);

    StringView string_view3{};
    string_view3 = Cxx::move(string_view);
    VERIFY(string_view.is_null_or_empty());
    VERIFY_EQUAL(string_view3.len(), 11);
    VERIFY_FALSE(string_view3.is_null_or_empty());
    VERIFY_EQUAL(string_view3, "Hello World"sv);
}

TEST_CASE(swap) {
    auto string_view  = "Hello everyone"sv;
    auto string_view2 = "Hi everyone"sv;

    VERIFY_EQUAL(string_view, "Hello everyone"sv);
    VERIFY_EQUAL(string_view2, "Hi everyone"sv);

    string_view.swap(string_view2);

    VERIFY_EQUAL(string_view2, "Hello everyone"sv);
    VERIFY_EQUAL(string_view, "Hi everyone"sv);
}

TEST_CASE(at_and_braces) {
    auto const string_view = "MeetixOS C++"sv;
    VERIFY_EQUAL(string_view[0], 'M');
    VERIFY_EQUAL(string_view[1], 'e');
    VERIFY_EQUAL(string_view[2], 'e');
    VERIFY_EQUAL(string_view[3], 't');
    VERIFY_EQUAL(string_view[4], 'i');
    VERIFY_EQUAL(string_view[5], 'x');
    VERIFY_EQUAL(string_view[6], 'O');
    VERIFY_EQUAL(string_view[7], 'S');
    VERIFY_EQUAL(string_view[8], ' ');
    VERIFY_EQUAL(string_view[9], 'C');
    VERIFY_EQUAL(string_view[10], '+');
    VERIFY_EQUAL(string_view[11], '+');

    VERIFY_EQUAL(string_view.at(0), 'M');
    VERIFY_EQUAL(string_view.at(1), 'e');
    VERIFY_EQUAL(string_view.at(2), 'e');
    VERIFY_EQUAL(string_view.at(3), 't');
    VERIFY_EQUAL(string_view.at(4), 'i');
    VERIFY_EQUAL(string_view.at(5), 'x');
    VERIFY_EQUAL(string_view.at(6), 'O');
    VERIFY_EQUAL(string_view.at(7), 'S');
    VERIFY_EQUAL(string_view.at(8), ' ');
    VERIFY_EQUAL(string_view.at(9), 'C');
    VERIFY_EQUAL(string_view.at(10), '+');
    VERIFY_EQUAL(string_view.at(11), '+');
}

TEST_CASE(compare) {
    auto const string_view = "Hello world OSDev"sv;

    VERIFY_EQUAL(string_view.compare("Hello"sv), 1);
    VERIFY_EQUAL(string_view.compare(""sv), 1);
    VERIFY_EQUAL(string_view.compare("Hello world OSDev"sv), 0);
    VERIFY_EQUAL(string_view.compare("Hello world OSDeveloper"sv), -1);

    auto const less_string_view  = "Hello"sv;
    auto const more_string_view  = "Hello world OSDeveloper"sv;
    auto const equal_string_view = "Hello World OSDev"sv;

    VERIFY_LESS(less_string_view, string_view);
    VERIFY_GREATER(more_string_view, string_view);
    VERIFY_LESS_EQUAL(equal_string_view, string_view);
    VERIFY_GREATER_EQUAL(equal_string_view, string_view);
}

TEST_CASE(equals_ignore_case) {
    auto const string_view = "Next generation Operating System"sv;

    VERIFY(string_view.equals_ignore_case("nExT GeneRAtIoN OpERatiNg SYStem"sv));
}

TEST_CASE(sub_string) {
    auto const string_view = "SplittableStringView"sv;

    VERIFY_EQUAL(string_view.sub_string_view(10), "StringView"sv);
    VERIFY_EQUAL(string_view.sub_string_view(3), "ittableStringView"sv);

    VERIFY_EQUAL(string_view.sub_string_view(5, 3), "tab"sv);
    VERIFY_EQUAL(string_view.sub_string_view(10, 6), "String"sv);
}

TEST_CASE(trim) {
    auto const string_view = "   \tYeah Buddy! \n \f \r"sv;

    VERIFY_EQUAL(string_view.trim_whitespaces(), "Yeah Buddy!"sv);

    auto const string_view2 = "---Yeah Buddy+++"sv;
    VERIFY_EQUAL(string_view2.trim("-+"sv, TrimMode::Left), "Yeah Buddy+++"sv);
    VERIFY_EQUAL(string_view2.trim("-+"sv, TrimMode::Right), "---Yeah Buddy"sv);
    VERIFY_EQUAL(string_view2.trim("-+"sv, TrimMode::Both), "Yeah Buddy"sv);
}

TEST_CASE(starts_with) {
    auto const string_view = "Hi I'm a StringView test"sv;

    VERIFY(string_view.starts_with('H'));
    VERIFY(string_view.starts_with('h', CaseSensitivity::Insensitive));
    VERIFY(string_view.starts_with("Hi"sv));
    VERIFY(string_view.starts_with("hi i'm a"sv, CaseSensitivity::Insensitive));
}

TEST_CASE(ends_with) {
    auto const string_view = "Beautiful world and beautiful country"sv;

    VERIFY(string_view.ends_with('y'));
    VERIFY(string_view.ends_with('Y', CaseSensitivity::Insensitive));
    VERIFY(string_view.ends_with("country"sv));
    VERIFY(string_view.ends_with("BeAuTifUl cOuNtRy"sv, CaseSensitivity::Insensitive));
}

TEST_CASE(as_int) {
    VERIFY_IS_PRESENT_EQUAL("127"sv.as_int<i8>(), 127);
    VERIFY_IS_PRESENT_EQUAL("-32"sv.as_int<i8>(), -32);
    VERIFY_IS_PRESENT_EQUAL("450\n"sv.as_int(), 450);
    VERIFY_IS_PRESENT_EQUAL("-65536"sv.as_int(), -65536);
    VERIFY_IS_NONE("\n 75"sv.as_int<i8>(TrimWhitespace::No));
    VERIFY_IS_NONE("\n 75"sv.as_int<i8>(TrimWhitespace::No));
    VERIFY_IS_NONE("7p5"sv.as_int<i8>());
    VERIFY_IS_NONE("512"sv.as_int<i8>());
}

TEST_CASE(as_uint) {
    VERIFY_IS_PRESENT_EQUAL("9753"sv.as_uint<u16>(), 9753);
    VERIFY_IS_PRESENT_EQUAL("6542037"sv.as_uint<u64>(), 6542037);
    VERIFY_IS_PRESENT_EQUAL("000917"sv.as_uint<u64>(), 917);

    VERIFY_IS_NONE("-64"sv.as_uint<u16>());
    VERIFY_IS_NONE("56AC"sv.as_uint<u64>());
}

TEST_CASE(as_uint_from_hex) {
    VERIFY_IS_PRESENT_EQUAL("0xabc45"sv.as_uint_from_hex(), 0xabc45);
    VERIFY_IS_PRESENT_EQUAL("1ff24bc"sv.as_uint_from_hex(), 0x1ff24bc);

    VERIFY_IS_NONE("3adg45"sv.as_uint_from_hex());
    VERIFY_IS_NONE("0x"sv.as_uint_from_hex());
}

TEST_CASE(as_uint_from_octal) {
    VERIFY_IS_PRESENT_EQUAL("05672"sv.as_uint_from_octal(), 05672);
    VERIFY_IS_PRESENT_EQUAL("3546"sv.as_uint_from_octal(), 03546);

    VERIFY_IS_NONE("458"sv.as_uint_from_octal());
}

TEST_CASE(find) {
    auto const string_view = "Hi everyone, this is a string_view"sv;

    VERIFY_IS_PRESENT_EQUAL(string_view.find('i'), 1);
    VERIFY_IS_PRESENT_EQUAL(string_view.find('a'), 21);
    VERIFY_IS_PRESENT_EQUAL(string_view.find("this"sv), 13);
    VERIFY_IS_PRESENT_EQUAL(string_view.find("tr"sv), 24);
    VERIFY_IS_NONE(string_view.find('a', 35));
}

TEST_CASE(find_last) {
    auto const string_view = "Hi everyone, this is a string_view"sv;

    VERIFY_IS_PRESENT_EQUAL(string_view.find_last('i'), 31);
    VERIFY_IS_PRESENT_EQUAL(string_view.find_last('a'), 21);
    VERIFY_IS_NONE(string_view.find_last('z'));
}

TEST_CASE(find_all) {
    auto const string_view = "AbaCibbo Holy AbaCucca ly"sv;

    auto positions = string_view.find_all("Aba"sv);
    VERIFY_FALSE(positions.is_empty());
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 0);
    VERIFY_EQUAL(positions.last(), 14);

    positions = string_view.find_all("xx"sv);
    VERIFY(positions.is_empty());

    auto error_or_positions = string_view.try_find_all("ly"sv);
    VERIFY(error_or_positions.is_value());

    positions = error_or_positions.unwrap_value();
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 11);
    VERIFY_EQUAL(positions.last(), 23);
}

TEST_CASE(contains) {
    auto const string_view = "Hi to everyone 1,2,3,4"sv;

    VERIFY(string_view.contains('1'));
    VERIFY(string_view.contains('e'));
    VERIFY(string_view.contains('E', CaseSensitivity::Insensitive));
    VERIFY_FALSE(string_view.contains('x'));
    VERIFY_FALSE(string_view.contains('E', CaseSensitivity::Sensitive));

    VERIFY(string_view.contains("to"sv));
    VERIFY(string_view.contains("2,3,4"sv));
    VERIFY(string_view.contains("eVeRyoNe"sv, CaseSensitivity::Insensitive));
    VERIFY_FALSE(string_view.contains("not"sv));
    VERIFY_FALSE(string_view.contains("HI"sv, CaseSensitivity::Sensitive));
}

TEST_CASE(iterator) {
    auto const string_view = "0123456789"sv;

    char i = '0';
    for ( auto const c : string_view )
        VERIFY_EQUAL(c, i++);
}

TEST_CASE(reverse_iterator) {
    auto const string_view = "9876543210"sv;

    char i = '0';
    for ( auto const c : string_view.reverse_iter() )
        VERIFY_EQUAL(c, i++);

    VERIFY_EQUAL(i, '9' + 1);
}