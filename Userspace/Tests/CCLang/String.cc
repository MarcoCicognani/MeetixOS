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

#include <cstring>
#include <CCLang/Alloc/String.hh>
#include <CCLang/Lang/StringView.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(try_construct_from) {
    auto error_or_empty_string = String::try_construct_from_view(""sv);
    verify$(error_or_empty_string.is_value());

    auto const empty_string = error_or_empty_string.unwrap_value();
    verify$(empty_string.is_empty());
    verify_equal$(empty_string.len(), 0);
    verify_equal$(empty_string, ""sv);

    auto error_or_filled_string = String::try_construct_from_view("MeetixOS C++"sv);
    verify$(error_or_filled_string.is_value());

    auto const filled_string = error_or_filled_string.unwrap_value();
    verify_false$(filled_string.is_empty());
    verify_equal$(filled_string.len(), 12);
    verify_equal$(filled_string, "MeetixOS C++"sv);

    auto const string_view = "Hello World Gun!"sv;

    auto error_or_string_from_view = String::try_construct_from_view(string_view);
    verify$(error_or_string_from_view.is_value());

    auto string_from_view = error_or_string_from_view.unwrap_value();
    verify_false$(string_from_view.is_empty());
    verify_equal$(string_from_view.len(), 16);
    verify_equal$(string_from_view, "Hello World Gun!"sv);
}

TEST_CASE(swap) {
    auto string   = String::construct_from_view("Hello World!"sv);
    auto string_2 = String::construct_from_view("Oh My God!"sv);

    verify_equal$(string, "Hello World!"sv);
    verify_equal$(string_2, "Oh My God!"sv);

    string.swap(string_2);

    verify_equal$(string, "Oh My God!"sv);
    verify_equal$(string_2, "Hello World!"sv);
}

TEST_CASE(at_and_braces) {
    auto const string = String::construct_from_view("MeetixOS C++"sv);
    verify_equal$(string[0], 'M');
    verify_equal$(string[1], 'e');
    verify_equal$(string[2], 'e');
    verify_equal$(string[3], 't');
    verify_equal$(string[4], 'i');
    verify_equal$(string[5], 'x');
    verify_equal$(string[6], 'O');
    verify_equal$(string[7], 'S');
    verify_equal$(string[8], ' ');
    verify_equal$(string[9], 'C');
    verify_equal$(string[10], '+');
    verify_equal$(string[11], '+');

    verify_equal$(string.at(0), 'M');
    verify_equal$(string.at(1), 'e');
    verify_equal$(string.at(2), 'e');
    verify_equal$(string.at(3), 't');
    verify_equal$(string.at(4), 'i');
    verify_equal$(string.at(5), 'x');
    verify_equal$(string.at(6), 'O');
    verify_equal$(string.at(7), 'S');
    verify_equal$(string.at(8), ' ');
    verify_equal$(string.at(9), 'C');
    verify_equal$(string.at(10), '+');
    verify_equal$(string.at(11), '+');
}

TEST_CASE(compare) {
    auto const string = String::construct_from_view("Hello world OSDev"sv);

    verify_equal$(string.compare("Hello"sv), 1);
    verify_equal$(string.compare(""sv), 1);
    verify_equal$(string.compare("Hello world OSDev"sv), 0);
    verify_equal$(string.compare("Hello world OSDeveloper"sv), -1);

    auto const less_string  = String::construct_from_view("Hello"sv);
    auto const more_string  = String::construct_from_view("Hello world OSDeveloper"sv);
    auto const equal_string = String::construct_from_view("Hello World OSDev"sv);

    verify_less$(less_string, string);
    verify_greater$(more_string, string);
    verify_less_equal$(equal_string, string);
    verify_greater_equal$(equal_string, string);
}

TEST_CASE(equals_ignore_case) {
    auto const string = String::construct_from_view("Next generation Operating System"sv);

    verify$(string.equals_ignore_case("nExT GeneRAtIoN OpERatiNg SYStem"sv));
}

TEST_CASE(sub_string) {
    auto const string = String::construct_from_view("SplittableString"sv);

    verify_equal$(string.sub_string(10), "String"sv);
    verify_equal$(string.sub_string(3, 10), "ittableStr"sv);

    verify_equal$(string.sub_string_view(5, 3), "tab"sv);
    verify_equal$(string.sub_string_view(10, 6), "String"sv);

    VERIFY_IS_VALUE_EQUAL(string.try_sub_string(12), "ring"sv);
}

TEST_CASE(trim) {
    auto const string = String::construct_from_view("   \tYeah Buddy! \n \f \r"sv);
    verify_equal$(string.trim_whitespaces(), "Yeah Buddy!"sv);

    auto const string_2 = String::construct_from_view("---Yeah Buddy+++"sv);
    verify_equal$(string_2.trim("-+"sv, String::TrimMode::Left), "Yeah Buddy+++"sv);
    verify_equal$(string_2.trim("-+"sv, String::TrimMode::Right), "---Yeah Buddy"sv);
    verify_equal$(string_2.trim("-+"sv, String::TrimMode::Both), "Yeah Buddy"sv);

    auto const string_3 = String::construct_from_view("  \vDestroy  \nEverything "sv);
    VERIFY_IS_VALUE_EQUAL(string_3.try_trim_whitespaces(), "Destroy  \nEverything"sv);

    auto const string_4 = String::construct_from_view("//////Path/To/Sub....."sv);
    VERIFY_IS_VALUE_EQUAL(string_4.try_trim("./"sv), "Path/To/Sub"sv);
}

TEST_CASE(starts_with) {
    auto const string = String::construct_from_view("Hi I'm a StringView test"sv);

    verify$(string.starts_with('H'));
    verify$(string.starts_with('h', String::CaseSensible::No));
    verify$(string.starts_with("Hi"sv));
    verify$(string.starts_with("hi i'm a"sv, String::CaseSensible::No));
}

TEST_CASE(ends_with) {
    auto const string = String::construct_from_view("Beautiful world and beautiful country"sv);

    verify$(string.ends_with('y'));
    verify$(string.ends_with('Y', String::CaseSensible::No));
    verify$(string.ends_with("country"sv));
    verify$(string.ends_with("BeAuTifUl cOuNtRy"sv, String::CaseSensible::No));
}

TEST_CASE(as_int) {
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("127"sv).as_int<i8>(), 127);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("-32"sv).as_int<i8>(), -32);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("450\n"sv).as_int(), 450);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("-65536"sv).as_int(), -65536);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("  \t64begin"sv).as_int(String::IntBase::Decimal, String::ParseMode::TrimWhitesAndBegin), 64);

    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("0xabc45"sv).as_int<i32>(String::IntBase::Hex), 0xabc45);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("-1ff24bc"sv).as_int<i32>(String::IntBase::Hex), -0x1ff24bc);

    VERIFY_IS_ERROR_EQUAL(String::construct_from_view("\n 75"sv).as_int<i8>(String::IntBase::Decimal, String::ParseMode::BeginToEnd), EINVAL);
    VERIFY_IS_ERROR_EQUAL(String::construct_from_view("7p5"sv).as_int<i8>(String::IntBase::Decimal), EINVAL);
    VERIFY_IS_ERROR_EQUAL(String::construct_from_view("512"sv).as_int<i8>(String::IntBase::Decimal), EOVERFLOW);
}

TEST_CASE(as_uint) {
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("9753"sv).as_uint<u16>(), 9753);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("6542037"sv).as_uint<u64>(), 6542037);
    VERIFY_IS_VALUE_EQUAL(String::construct_from_view("000917"sv).as_uint<u64>(), 917);

    VERIFY_IS_ERROR_EQUAL(String::construct_from_view("-64"sv).as_uint<u16>(), EINVAL);
    VERIFY_IS_ERROR_EQUAL(String::construct_from_view("56AC"sv).as_uint<u64>(), EINVAL);
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
    verify_false$(positions.is_empty());
    verify_equal$(positions.count(), 2);
    verify_equal$(positions.first(), 0);
    verify_equal$(positions.last(), 14);

    positions = string.find_all("xx"sv);
    verify$(positions.is_empty());

    auto error_or_positions = string.try_find_all("ly"sv);
    verify$(error_or_positions.is_value());

    positions = error_or_positions.unwrap_value();
    verify_equal$(positions.count(), 2);
    verify_equal$(positions.first(), 11);
    verify_equal$(positions.last(), 23);
}

TEST_CASE(to_lowercase) {
    verify_equal$(String::construct_from_view("HI TO EVERYONE"sv).to_lowercase(), "hi to everyone"sv);
    verify_equal$(String::construct_from_view("hi to everyone"sv).to_lowercase(), "hi to everyone"sv);
    verify_equal$(String::construct_from_view("Hi tO EvErYOnE"sv).to_lowercase(), "hi to everyone"sv);
    verify_equal$(String::construct_from_view("Hi tO 6 % $ EvErYOnE 67"sv).to_lowercase(), "hi to 6 % $ everyone 67"sv);
}

TEST_CASE(to_uppercase) {
    verify_equal$(String::construct_from_view("hi to everyone"sv).to_uppercase(), "HI TO EVERYONE"sv);
    verify_equal$(String::construct_from_view("HI TO EVERYONE"sv).to_uppercase(), "HI TO EVERYONE"sv);
    verify_equal$(String::construct_from_view("Hi tO EvErYOnE"sv).to_uppercase(), "HI TO EVERYONE"sv);
    verify_equal$(String::construct_from_view("Hi tO 6 % $ EvErYOnE 67"sv).to_uppercase(), "HI TO 6 % $ EVERYONE 67"sv);
}

TEST_CASE(to_reverse) {
    auto const string = String::construct_from_view("hello everyone"sv).to_reverse();
    char buf[512];
    std::strncpy(buf, string.as_cstr(), string.len());
    printf("-> \"%s\"\n", buf);
    verify_equal$(String::construct_from_view("hello everyone"sv).to_reverse(), "enoyreve olleh"sv);
    verify_equal$(String::construct_from_view("1234567"sv).to_reverse(), "7654321"sv);
}

TEST_CASE(contains) {
    auto const string = String::construct_from_view("Hi to everyone 1,2,3,4"sv);

    verify$(string.contains('1'));
    verify$(string.contains('e'));
    verify$(string.contains('E', String::CaseSensible::No));
    verify_false$(string.contains('x'));
    verify_false$(string.contains('E', String::CaseSensible::Yes));

    verify$(string.contains("to"sv));
    verify$(string.contains("2,3,4"sv));
    verify$(string.contains("eVeRyoNe"sv, String::CaseSensible::No));
    verify_false$(string.contains("not"sv));
    verify_false$(string.contains("HI"sv, String::CaseSensible::Yes));
}

TEST_CASE(iterator) {
    auto const string = String::construct_from_view("0123456789"sv);

    char i = '0';
    for ( auto const c : string )
        verify_equal$(c, i++);
}

TEST_CASE(reverse_iterator) {
    auto const string = String::construct_from_view("9876543210"sv);

    char i = '0';
    for ( auto const c : string.reverse_iter() )
        verify_equal$(c, i++);

    verify_equal$(i, '9' + 1);
}