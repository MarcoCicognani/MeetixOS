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
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(try_construct_from) {
    auto error_or_empty_string = String::try_construct_from_view(""sv);
    VERIFY(error_or_empty_string.is_value());

    auto const empty_string = error_or_empty_string.unwrap_value();
    VERIFY(empty_string.is_empty());
    VERIFY_EQUAL(empty_string.len(), 0);
    VERIFY_EQUAL(empty_string, ""sv);

    auto error_or_filled_string = String::try_construct_from_view("MeetixOS C++"sv);
    VERIFY(error_or_filled_string.is_value());

    auto const filled_string = error_or_filled_string.unwrap_value();
    VERIFY_FALSE(filled_string.is_empty());
    VERIFY_EQUAL(filled_string.len(), 12);
    VERIFY_EQUAL(filled_string, "MeetixOS C++"sv);

    auto const string_view = "Hello World Gun!"sv;

    auto error_or_string_from_view = String::try_construct_from_view(string_view);
    VERIFY(error_or_string_from_view.is_value());

    auto string_from_view = error_or_string_from_view.unwrap_value();
    VERIFY_FALSE(string_from_view.is_empty());
    VERIFY_EQUAL(string_from_view.len(), 16);
    VERIFY_EQUAL(string_from_view, "Hello World Gun!"sv);
}

TEST_CASE(swap) {
    auto string   = String::construct_from_view("Hello World!"sv);
    auto string_2 = String::construct_from_view("Oh My God!"sv);

    VERIFY_EQUAL(string, "Hello World!"sv);
    VERIFY_EQUAL(string_2, "Oh My God!"sv);

    string.swap(string_2);

    VERIFY_EQUAL(string, "Oh My God!"sv);
    VERIFY_EQUAL(string_2, "Hello World!"sv);
}

TEST_CASE(at_and_braces) {
    auto const string = String::construct_from_view("MeetixOS C++"sv);
    VERIFY_EQUAL(string[0], 'M');
    VERIFY_EQUAL(string[1], 'e');
    VERIFY_EQUAL(string[2], 'e');
    VERIFY_EQUAL(string[3], 't');
    VERIFY_EQUAL(string[4], 'i');
    VERIFY_EQUAL(string[5], 'x');
    VERIFY_EQUAL(string[6], 'O');
    VERIFY_EQUAL(string[7], 'S');
    VERIFY_EQUAL(string[8], ' ');
    VERIFY_EQUAL(string[9], 'C');
    VERIFY_EQUAL(string[10], '+');
    VERIFY_EQUAL(string[11], '+');

    VERIFY_EQUAL(string.at(0), 'M');
    VERIFY_EQUAL(string.at(1), 'e');
    VERIFY_EQUAL(string.at(2), 'e');
    VERIFY_EQUAL(string.at(3), 't');
    VERIFY_EQUAL(string.at(4), 'i');
    VERIFY_EQUAL(string.at(5), 'x');
    VERIFY_EQUAL(string.at(6), 'O');
    VERIFY_EQUAL(string.at(7), 'S');
    VERIFY_EQUAL(string.at(8), ' ');
    VERIFY_EQUAL(string.at(9), 'C');
    VERIFY_EQUAL(string.at(10), '+');
    VERIFY_EQUAL(string.at(11), '+');
}

TEST_CASE(compare) {
    auto const string = String::construct_from_view("Hello world OSDev"sv);

    VERIFY_EQUAL(string.compare("Hello"sv), 1);
    VERIFY_EQUAL(string.compare(""sv), 1);
    VERIFY_EQUAL(string.compare("Hello world OSDev"sv), 0);
    VERIFY_EQUAL(string.compare("Hello world OSDeveloper"sv), -1);

    auto const less_string  = String::construct_from_view("Hello"sv);
    auto const more_string  = String::construct_from_view("Hello world OSDeveloper"sv);
    auto const equal_string = String::construct_from_view("Hello World OSDev"sv);

    VERIFY_LESS(less_string, string);
    VERIFY_GREATER(more_string, string);
    VERIFY_LESS_EQUAL(equal_string, string);
    VERIFY_GREATER_EQUAL(equal_string, string);
}

TEST_CASE(equals_ignore_case) {
    auto const string = String::construct_from_view("Next generation Operating System"sv);

    VERIFY(string.equals_ignore_case("nExT GeneRAtIoN OpERatiNg SYStem"sv));
}

TEST_CASE(sub_string) {
    auto const string = String::construct_from_view("SplittableString"sv);

    VERIFY_EQUAL(string.sub_string(10), "String"sv);
    VERIFY_EQUAL(string.sub_string(3, 10), "ittableStr"sv);

    VERIFY_EQUAL(string.sub_string_view(5, 3), "tab"sv);
    VERIFY_EQUAL(string.sub_string_view(10, 6), "String"sv);

    VERIFY_IS_VALUE_EQUAL(string.try_sub_string(12), "ring"sv);
}

TEST_CASE(trim) {
    auto const string = String::construct_from_view("   \tYeah Buddy! \n \f \r"sv);
    VERIFY_EQUAL(string.trim_whitespaces(), "Yeah Buddy!"sv);

    auto const string_2 = String::construct_from_view("---Yeah Buddy+++"sv);
    VERIFY_EQUAL(string_2.trim("-+"sv, TrimMode::Left), "Yeah Buddy+++"sv);
    VERIFY_EQUAL(string_2.trim("-+"sv, TrimMode::Right), "---Yeah Buddy"sv);
    VERIFY_EQUAL(string_2.trim("-+"sv, TrimMode::Both), "Yeah Buddy"sv);

    auto const string_3 = String::construct_from_view("  \vDestroy  \nEverything "sv);
    VERIFY_IS_VALUE_EQUAL(string_3.try_trim_whitespaces(), "Destroy  \nEverything"sv);

    auto const string_4 = String::construct_from_view("//////Path/To/Sub....."sv);
    VERIFY_IS_VALUE_EQUAL(string_4.try_trim("./"sv), "Path/To/Sub"sv);
}

TEST_CASE(starts_with) {
    auto const string = String::construct_from_view("Hi I'm a StringView test"sv);

    VERIFY(string.starts_with('H'));
    VERIFY(string.starts_with('h', CaseSensitivity::Insensitive));
    VERIFY(string.starts_with("Hi"sv));
    VERIFY(string.starts_with("hi i'm a"sv, CaseSensitivity::Insensitive));
}

TEST_CASE(ends_with) {
    auto const string = String::construct_from_view("Beautiful world and beautiful country"sv);

    VERIFY(string.ends_with('y'));
    VERIFY(string.ends_with('Y', CaseSensitivity::Insensitive));
    VERIFY(string.ends_with("country"sv));
    VERIFY(string.ends_with("BeAuTifUl cOuNtRy"sv, CaseSensitivity::Insensitive));
}

TEST_CASE(as_int) {
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("67"sv).as_int<i8>(), 67);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("-43"sv).as_int<i8>(), -43);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("\n 88"sv).as_int<i8>(), 88);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view(" \v \r89\n"sv).as_int(), 89);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("-89654"sv).as_int(), -89654);
    VERIFY_IS_NONE(String::construct_from_view("\n 88"sv).as_int<i8>(TrimWhitespace::No));
    VERIFY_IS_NONE(String::construct_from_view("384"sv).as_int<i8>());
}

TEST_CASE(as_uint) {
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("6752"sv).as_uint<u16>(), 6752);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("8652164"sv).as_uint<u64>(), 8652164);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("00000657"sv).as_uint<u64>(), 657);
    VERIFY_IS_NONE(String::construct_from_view("-35"sv).as_uint());
    VERIFY_IS_NONE(String::construct_from_view("56x7J9"sv).as_uint());
}

TEST_CASE(as_uint_from_hex) {
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("0x1a2b3c"sv).as_uint_from_hex(), 0x1a2b3c);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("0f6d7a"sv).as_uint_from_hex(), 0x0f6d7a);
    VERIFY_IS_NONE(String::construct_from_view("i94hr"sv).as_uint_from_hex());
    VERIFY_IS_NONE(String::construct_from_view("0x"sv).as_uint_from_hex());
}

TEST_CASE(as_uint_from_octal) {
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("0612"sv).as_uint_from_octal(), 0612);
    VERIFY_IS_PRESENT_EQUAL(String::construct_from_view("35172"sv).as_uint_from_octal(), 035172);
    VERIFY_IS_NONE(String::construct_from_view("12634529"sv).as_uint_from_octal());
}

TEST_CASE(find) {
    auto const string = String::construct_from_view("Hi everyone, this is a string"sv);

    VERIFY_IS_PRESENT_EQUAL(string.find('i'), 1);
    VERIFY_IS_PRESENT_EQUAL(string.find('a'), 21);
    VERIFY_IS_PRESENT_EQUAL(string.find("this"sv), 13);
    VERIFY_IS_PRESENT_EQUAL(string.find("tr"sv), 24);
    VERIFY_IS_PRESENT_EQUAL(string.find('a'), 21);
    VERIFY_IS_NONE(string.find('a', 35));
}

TEST_CASE(find_last) {
    auto const string = String::construct_from_view("Hi everyone, this is a string"sv);

    VERIFY_IS_PRESENT_EQUAL(string.find_last('i'), 26);
    VERIFY_IS_PRESENT_EQUAL(string.find_last('a'), 21);
    VERIFY_IS_NONE(string.find_last('z'));
}

TEST_CASE(find_all) {
    auto const string = String::construct_from_view("AbaCibbo Holy AbaCucca ly"sv);

    auto positions = string.find_all("Aba"sv);
    VERIFY_FALSE(positions.is_empty());
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 0);
    VERIFY_EQUAL(positions.last(), 14);

    positions = string.find_all("xx"sv);
    VERIFY(positions.is_empty());

    auto error_or_positions = string.try_find_all("ly"sv);
    VERIFY(error_or_positions.is_value());

    positions = error_or_positions.unwrap_value();
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 11);
    VERIFY_EQUAL(positions.last(), 23);
}

TEST_CASE(to_lowercase) {
    VERIFY_EQUAL(String::construct_from_view("HI TO EVERYONE"sv).to_uppercase(), "hi to everyone"sv);
    VERIFY_EQUAL(String::construct_from_view("hi to everyone"sv).to_uppercase(), "hi to everyone"sv);
    VERIFY_EQUAL(String::construct_from_view("Hi tO EvErYOnE"sv).to_uppercase(), "hi to everyone"sv);
    VERIFY_EQUAL(String::construct_from_view("Hi tO 6 % $ EvErYOnE 67"sv).to_uppercase(), "hi to 6 % $ everyone 67"sv);
}

TEST_CASE(to_uppercase) {
    VERIFY_EQUAL(String::construct_from_view("hi to everyone"sv).to_uppercase(), "HI TO EVERYONE"sv);
    VERIFY_EQUAL(String::construct_from_view("HI TO EVERYONE"sv).to_uppercase(), "HI TO EVERYONE"sv);
    VERIFY_EQUAL(String::construct_from_view("Hi tO EvErYOnE"sv).to_uppercase(), "HI TO EVERYONE"sv);
    VERIFY_EQUAL(String::construct_from_view("Hi tO 6 % $ EvErYOnE 67"sv).to_uppercase(), "HI TO 6 % $ EVERYONE 67"sv);
}

TEST_CASE(contains) {
    auto const string = String::construct_from_view("Hi to everyone 1,2,3,4"sv);

    VERIFY(string.contains('1'));
    VERIFY(string.contains('e'));
    VERIFY(string.contains('E', CaseSensitivity::Insensitive));
    VERIFY_FALSE(string.contains('x'));
    VERIFY_FALSE(string.contains('E', CaseSensitivity::Sensitive));

    VERIFY(string.contains("to"sv));
    VERIFY(string.contains("2,3,4"sv));
    VERIFY(string.contains("eVeRyoNe"sv, CaseSensitivity::Insensitive));
    VERIFY_FALSE(string.contains("not"sv));
    VERIFY_FALSE(string.contains("HI"sv, CaseSensitivity::Sensitive));
}

TEST_CASE(iterator) {
    auto const string = String::construct_from_view("0123456789"sv);

    char i = '0';
    for ( auto c : string )
        VERIFY_EQUAL(c, i++);
}