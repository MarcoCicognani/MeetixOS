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

#include <TC/Collection/String.hh>
#include <TC/Collection/StringView.hh>
#include <TC/Cxx.hh>
#include <UnitTest/Assertions.hh>
#include <UnitTest/Case.hh>

using namespace TC;

TEST_CASE(construction) {
    StringView empty_string_view{};
    VERIFY(empty_string_view.is_empty());
    VERIFY(empty_string_view.is_null());
    VERIFY(empty_string_view.is_null_or_empty());
    VERIFY_EQUAL(empty_string_view, "");

    String     string{ "Testing LibTC Strings" };
    StringView from_string_string_view{ string };
    VERIFY_FALSE(from_string_string_view.is_null_or_empty());
    VERIFY_EQUAL(from_string_string_view.len(), 21);
    VERIFY_EQUAL(from_string_string_view.len(), string.len());
    VERIFY_EQUAL(from_string_string_view, "Testing LibTC Strings");
    VERIFY_EQUAL(from_string_string_view, "Testing LibTC Strings"sv);
    VERIFY_EQUAL(from_string_string_view, String{ "Testing LibTC Strings" });

    StringView from_cstr_string_view{ "Hi from MeetixOS LibTC Tests" };
    VERIFY_FALSE(from_cstr_string_view.is_null_or_empty());
    VERIFY_EQUAL(from_cstr_string_view.len(), 28);
    VERIFY_EQUAL(from_cstr_string_view, "Hi from MeetixOS LibTC Tests");
    VERIFY_EQUAL(from_cstr_string_view, "Hi from MeetixOS LibTC Tests"sv);
    VERIFY_EQUAL(from_cstr_string_view, String{ "Hi from MeetixOS LibTC Tests" });
}

TEST_CASE(assignment_operators) {
    StringView string_view{};
    VERIFY(string_view.is_null_or_empty());

    string_view = "Hello World";
    VERIFY_EQUAL(string_view.len(), 11);
    VERIFY_EQUAL(string_view, "Hello World");

    string_view = String{ "MeetixOS" };
    VERIFY_EQUAL(string_view.len(), 8);
    VERIFY_EQUAL(string_view, "MeetixOS");
    VERIFY_EQUAL(string_view, String{ "MeetixOS" });

    StringView string_view2{};
    string_view2 = string_view;
    VERIFY_FALSE(string_view.is_null_or_empty());
    VERIFY_FALSE(string_view2.is_null_or_empty());
    VERIFY_EQUAL(string_view, "MeetixOS");
    VERIFY_EQUAL(string_view2, "MeetixOS");

    StringView string_view3{};
    string_view3 = move(string_view);
    VERIFY(string_view.is_null_or_empty());
    VERIFY_EQUAL(string_view3.len(), 8);
    VERIFY_FALSE(string_view3.is_null_or_empty());
    VERIFY_EQUAL(string_view3, "MeetixOS"sv);
}

TEST_CASE(swap) {
    StringView string_view{ "Hello everyone" };
    StringView string_view2{ "Hi everyone" };

    VERIFY_EQUAL(string_view, "Hello everyone");
    VERIFY_EQUAL(string_view2, "Hi everyone");

    string_view.swap(string_view2);

    VERIFY_EQUAL(string_view2, "Hello everyone");
    VERIFY_EQUAL(string_view, "Hi everyone");
}

TEST_CASE(at_and_braces) {
    StringView string_view{ "MeetixOS C++" };
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
    StringView string_view{ "Hello world OSDev" };

    VERIFY_EQUAL(string_view.compare("Hello"), 1);
    VERIFY_EQUAL(string_view.compare(""), 1);
    VERIFY_EQUAL(string_view.compare("Hello world OSDev"), 0);
    VERIFY_EQUAL(string_view.compare("Hello world OSDeveloper"), -1);

    StringView less_string_view{ "Hello" };
    StringView more_string_view{ "Hello world OSDeveloper" };
    StringView equal_string_view{ "Hello World OSDev" };

    VERIFY_LESS(less_string_view, string_view);
    VERIFY_GREATER(more_string_view, string_view);
    VERIFY_LESS_EQUAL(equal_string_view, string_view);
    VERIFY_GREATER_EQUAL(equal_string_view, string_view);
}

TEST_CASE(equals_ignore_case) {
    StringView string_view{ "Next generation Operating System" };

    VERIFY(string_view.equals_ignore_case("nExT GeneRAtIoN OpERatiNg SYStem"));
}

TEST_CASE(sub_string) {
    StringView string_view{ "SplittableStringView" };

    VERIFY_EQUAL(string_view.sub_string(10), "StringView"sv);
    VERIFY_EQUAL(string_view.sub_string(3), "ittableStringView"sv);

    VERIFY_EQUAL(string_view.sub_string(5, 3), "tab"sv);
    VERIFY_EQUAL(string_view.sub_string(10, 6), "String"sv);
}

TEST_CASE(trim) {
    StringView string_view{ "   \tYeah Buddy! \n \f \r" };

    VERIFY_EQUAL(string_view.trim_whitespaces(), "Yeah Buddy!"sv);

    StringView string_view2{ "---Yeah Buddy+++" };
    VERIFY_EQUAL(string_view2.trim("-+", TrimMode::Left), "Yeah Buddy+++"sv);
    VERIFY_EQUAL(string_view2.trim("-+", TrimMode::Right), "---Yeah Buddy"sv);
    VERIFY_EQUAL(string_view2.trim("-+", TrimMode::Both), "Yeah Buddy"sv);
}

TEST_CASE(starts_with) {
    StringView string_view{};
    string_view = "Hi I'm a StringView test";

    VERIFY(string_view.starts_with('H'));
    VERIFY(string_view.starts_with('h', CaseSensitivity::Insensitive));
    VERIFY(string_view.starts_with("Hi"));
    VERIFY(string_view.starts_with("hi i'm a", CaseSensitivity::Insensitive));
}

TEST_CASE(ends_with) {
    StringView string_view{ "Beautiful world and beautiful country" };

    VERIFY(string_view.ends_with('y'));
    VERIFY(string_view.ends_with('Y', CaseSensitivity::Insensitive));
    VERIFY(string_view.ends_with("country"));
    VERIFY(string_view.ends_with("BeAuTifUl cOuNtRy", CaseSensitivity::Insensitive));
}

TEST_CASE(as_int) {
    StringView i8_string_view{ "127" };
    auto       i8_or_none = i8_string_view.as_int<i8>();
    VERIFY(i8_or_none.is_present());
    VERIFY_EQUAL(i8_or_none.value(), 127);

    i8_string_view = "-32";
    i8_or_none     = i8_string_view.as_int<i8>();
    VERIFY(i8_or_none.is_present());
    VERIFY_EQUAL(i8_or_none.value(), -32);

    i8_string_view = "\n 75";
    i8_or_none     = i8_string_view.as_int<i8>(TrimWhitespace::No);
    VERIFY_FALSE(i8_or_none.is_present());

    i8_string_view = "7p5";
    i8_or_none     = i8_string_view.as_int<i8>();
    VERIFY_FALSE(i8_or_none.is_present());

    i8_string_view = "512";
    i8_or_none     = i8_string_view.as_int<i8>();
    VERIFY_FALSE(i8_or_none.is_present());

    StringView i32_string_view{ " 450\n" };
    auto       i32_or_none = i32_string_view.as_int();
    VERIFY(i32_or_none.is_present());
    VERIFY_EQUAL(i32_or_none.value(), 450);

    i32_string_view = "-65536";
    i32_or_none     = i32_string_view.as_int();
    VERIFY(i32_or_none.is_present());
    VERIFY_EQUAL(i32_or_none.value(), -65536);
}

TEST_CASE(as_uint) {
    StringView u16_string_view{ "9753" };
    auto       u16_or_none = u16_string_view.as_uint<u16>();
    VERIFY(u16_or_none.is_present());
    VERIFY_EQUAL(u16_or_none.value(), 9753);

    u16_string_view = "-64";
    u16_or_none     = u16_string_view.as_uint<u16>();
    VERIFY_FALSE(u16_or_none.is_present());

    StringView u64_string_view{ "6542037" };
    auto       u64_or_none = u64_string_view.as_uint<u64>();
    VERIFY(u64_or_none.is_present());
    VERIFY_EQUAL(u64_or_none.value(), 6542037);

    u64_string_view = "000917";
    u64_or_none     = u64_string_view.as_uint<u64>();
    VERIFY(u64_or_none.is_present());
    VERIFY_EQUAL(u64_or_none.value(), 917);

    u64_string_view = "56AC";
    u64_or_none     = u64_string_view.as_uint<u64>();
    VERIFY_FALSE(u64_or_none.is_present());
}

TEST_CASE(as_uint_from_hex) {
    StringView string_view{ "0xabc45" };
    auto       u32_or_none = string_view.as_uint_from_hex();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 0xabc45);

    string_view = "1ff24bc";
    u32_or_none = string_view.as_uint_from_hex();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 0x1ff24bc);

    string_view = "3adg45";
    u32_or_none = string_view.as_uint_from_hex();
    VERIFY_FALSE(u32_or_none.is_present());

    string_view = "0x";
    u32_or_none = string_view.as_uint_from_hex();
    VERIFY_FALSE(u32_or_none.is_present());
}

TEST_CASE(as_uint_from_octal) {
    StringView string_view{ "05672" };
    auto       u32_or_none = string_view.as_uint_from_octal();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 05672);

    string_view = "3546";
    u32_or_none = string_view.as_uint_from_octal();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 03546);

    string_view = "458";
    u32_or_none = string_view.as_uint_from_octal();
    VERIFY_FALSE(u32_or_none.is_present());
}

TEST_CASE(find) {
    StringView string_view{ "Hi everyone, this is a string_view" };

    auto index_or_none = string_view.find('i');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 1);

    index_or_none = string_view.find('a');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 21);

    index_or_none = string_view.find('a', 35);
    VERIFY_FALSE(index_or_none.is_present());

    index_or_none = string_view.find("this");
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 13);

    index_or_none = string_view.find("tr");
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 24);
}

TEST_CASE(find_last) {
    StringView string_view{ "Hi everyone, this is a string_view" };

    auto index_or_none = string_view.find_last('i');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 31);

    index_or_none = string_view.find_last('a');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 21);

    index_or_none = string_view.find_last('z');
    VERIFY_FALSE(index_or_none.is_present());
}

TEST_CASE(find_all) {
    StringView string_view{ "AbaCibbo Holy AbaCucca ly" };

    auto positions = string_view.find_all("Aba");
    VERIFY_FALSE(positions.is_empty());
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 0);
    VERIFY_EQUAL(positions.last(), 14);

    positions = string_view.find_all("xx");
    VERIFY(positions.is_empty());

    auto error_or_positions = string_view.try_find_all("ly");
    VERIFY(error_or_positions.is_value());

    positions = error_or_positions.unwrap_value();
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 11);
    VERIFY_EQUAL(positions.last(), 23);
}

TEST_CASE(contains) {
    StringView string_view{ "Hi to everyone 1,2,3,4" };

    VERIFY(string_view.contains('1'));
    VERIFY(string_view.contains('e'));
    VERIFY(string_view.contains('E', CaseSensitivity::Insensitive));
    VERIFY_FALSE(string_view.contains('x'));
    VERIFY_FALSE(string_view.contains('E', CaseSensitivity::Sensitive));

    VERIFY(string_view.contains("to"));
    VERIFY(string_view.contains("2,3,4"));
    VERIFY(string_view.contains("eVeRyoNe", CaseSensitivity::Insensitive));
    VERIFY_FALSE(string_view.contains("not"));
    VERIFY_FALSE(string_view.contains("HI", CaseSensitivity::Sensitive));
}

TEST_CASE(iterator) {
    StringView string_view{ "0123456789" };

    char expected_value = '0';
    for ( auto c : string_view )
        VERIFY_EQUAL(c, expected_value++);
}