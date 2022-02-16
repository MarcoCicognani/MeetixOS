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

#include <LibTC/Collection/String.hh>
#include <LibTC/Cxx.hh>
#include <UnitTest/Assertions.hh>
#include <UnitTest/Case.hh>

using namespace TC;

TEST_CASE(try_from) {
    auto error_or_empty_string = String::try_from("");
    VERIFY(error_or_empty_string.is_value());

    auto empty_string = error_or_empty_string.unwrap_value();
    VERIFY(empty_string.is_empty());
    VERIFY_EQUAL(empty_string.len(), 0);
    VERIFY_EQUAL(empty_string, "");

    auto error_or_filled_string = String::try_from("MeetixOS C++");
    VERIFY(error_or_filled_string.is_value());

    auto filled_string = error_or_filled_string.unwrap_value();
    VERIFY_FALSE(filled_string.is_empty());
    VERIFY_EQUAL(filled_string.len(), 12);
    VERIFY_EQUAL(filled_string, "MeetixOS C++");

    auto error_or_sub_string = String::try_from("MeetixOS C++", 8);
    VERIFY(error_or_sub_string.is_value());

    auto sub_string = error_or_sub_string.unwrap_value();
    VERIFY_FALSE(sub_string.is_empty());
    VERIFY_EQUAL(sub_string.len(), 8);
    VERIFY_EQUAL(sub_string, "MeetixOS");

    StringView string_view{ "Hello World Gun!" };
    auto       error_or_string_from_view = String::try_from(string_view);
    VERIFY(error_or_string_from_view.is_value());

    auto string_from_view = error_or_string_from_view.unwrap_value();
    VERIFY_FALSE(string_from_view.is_empty());
    VERIFY_EQUAL(string_from_view.len(), 16);
    VERIFY_EQUAL(string_from_view, "Hello World Gun!"sv);
}

TEST_CASE(cxx_construction) {
    String empty_string{};
    VERIFY(empty_string.is_empty());
    VERIFY_EQUAL(empty_string.len(), 0);
    VERIFY_EQUAL(empty_string, "");

    String filled_string{ "MeetixOS C++" };
    VERIFY_FALSE(filled_string.is_empty());
    VERIFY_EQUAL(filled_string.len(), 12);
    VERIFY_EQUAL(filled_string, "MeetixOS C++");

    String sub_string{ "MeetixOS C++", 8 };
    VERIFY_FALSE(sub_string.is_empty());
    VERIFY_EQUAL(sub_string.len(), 8);
    VERIFY_EQUAL(sub_string, "MeetixOS");

    empty_string = "Hello world!";
    VERIFY_FALSE(empty_string.is_empty());
    VERIFY_EQUAL(empty_string, "Hello world!"sv);

    empty_string = "Hello OSDevelopers"sv;
    VERIFY_FALSE(empty_string.is_empty());
    VERIFY_EQUAL(empty_string, "Hello OSDevelopers"sv);
}

TEST_CASE(swap) {
    String string{ "Hello World!" };
    String string_2{ "Oh My God!" };

    VERIFY_EQUAL(string, "Hello World!");
    VERIFY_EQUAL(string_2, "Oh My God!");

    string.swap(string_2);

    VERIFY_EQUAL(string, "Oh My God!");
    VERIFY_EQUAL(string_2, "Hello World!");
}

TEST_CASE(at_and_braces) {
    String string{ "MeetixOS C++" };
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
    String string{ "Hello world OSDev" };

    VERIFY_EQUAL(string.compare("Hello"), 1);
    VERIFY_EQUAL(string.compare(""), 1);
    VERIFY_EQUAL(string.compare("Hello world OSDev"), 0);
    VERIFY_EQUAL(string.compare("Hello world OSDeveloper"), -1);

    String less_string{ "Hello" };
    String more_string{ "Hello world OSDeveloper" };
    String equal_string{ "Hello World OSDev" };

    VERIFY_LESS(less_string, string);
    VERIFY_GREATER(more_string, string);
    VERIFY_LESS_EQUAL(equal_string, string);
    VERIFY_GREATER_EQUAL(equal_string, string);
}

TEST_CASE(equals_ignore_case) {
    String string{ "Next generation Operating System" };

    VERIFY(string.equals_ignore_case("nExT GeneRAtIoN OpERatiNg SYStem"));
}

TEST_CASE(sub_string) {
    String string{ "SplittableString" };

    VERIFY_EQUAL(string.sub_string(10), "String");
    VERIFY_EQUAL(string.sub_string(3, 10), "ittableStr");

    VERIFY_EQUAL(string.sub_string_view(5, 3), "tab"sv);
    VERIFY_EQUAL(string.sub_string_view(10, 6), "String"sv);

    auto error_or_sub_string = string.try_sub_string(12);
    VERIFY(error_or_sub_string.is_value());

    auto sub_string = error_or_sub_string.unwrap_value();
    VERIFY_EQUAL(sub_string, "ring");
}

TEST_CASE(trim) {
    String string{ "   \tYeah Buddy! \n \f \r" };

    VERIFY_EQUAL(string.trim_whitespaces(), "Yeah Buddy!");

    String string_2{ "---Yeah Buddy+++" };
    VERIFY_EQUAL(string_2.trim("-+", TrimMode::Left), "Yeah Buddy+++");
    VERIFY_EQUAL(string_2.trim("-+", TrimMode::Right), "---Yeah Buddy");
    VERIFY_EQUAL(string_2.trim("-+", TrimMode::Both), "Yeah Buddy");

    String string_3{ "  \vDestroy  \nEverything " };

    auto error_or_trim_string = string_3.try_trim_whitespaces();
    VERIFY(error_or_trim_string.is_value());

    auto trim_string = error_or_trim_string.unwrap_value();
    VERIFY_EQUAL(trim_string, "Destroy  \nEverything");

    String string_4{ "//////Path/To/Sub....." };

    auto error_or_trim_string_2 = string_4.try_trim("./");
    VERIFY(error_or_trim_string_2.is_value());

    auto trim_string_2 = error_or_trim_string_2.unwrap_value();
    VERIFY_EQUAL(trim_string_2, "Path/To/Sub");
}

TEST_CASE(starts_with) {
    String string{};
    string = "Hi I'm a StringView test"sv;

    VERIFY(string.starts_with('H'));
    VERIFY(string.starts_with('h', CaseSensitivity::Insensitive));
    VERIFY(string.starts_with("Hi"));
    VERIFY(string.starts_with("hi i'm a", CaseSensitivity::Insensitive));
}

TEST_CASE(ends_with) {
    String string{ "Beautiful world and beautiful country" };

    VERIFY(string.ends_with('y'));
    VERIFY(string.ends_with('Y', CaseSensitivity::Insensitive));
    VERIFY(string.ends_with("country"));
    VERIFY(string.ends_with("BeAuTifUl cOuNtRy", CaseSensitivity::Insensitive));
}

TEST_CASE(as_int) {
    String i8_string{ "67" };
    auto   i8_or_none = i8_string.as_int<i8>();
    VERIFY(i8_or_none.is_present());
    VERIFY_EQUAL(i8_or_none.value(), 67);

    i8_string  = "-43";
    i8_or_none = i8_string.as_int<i8>();
    VERIFY(i8_or_none.is_present());
    VERIFY_EQUAL(i8_or_none.value(), -43);

    i8_string  = "\n 88";
    i8_or_none = i8_string.as_int<i8>(TrimWhitespace::No);
    VERIFY_FALSE(i8_or_none.is_present());

    i8_string  = "3l2r";
    i8_or_none = i8_string.as_int<i8>();
    VERIFY_FALSE(i8_or_none.is_present());

    i8_string  = "384";
    i8_or_none = i8_string.as_int<i8>();
    VERIFY_FALSE(i8_or_none.is_present());

    String i32_string{ " \v \r89\n" };
    auto   i32_or_none = i32_string.as_int();
    VERIFY(i32_or_none.is_present());
    VERIFY_EQUAL(i32_or_none.value(), 89);

    i32_string  = "-89654";
    i32_or_none = i32_string.as_int();
    VERIFY(i32_or_none.is_present());
    VERIFY_EQUAL(i32_or_none.value(), -89654);
}

TEST_CASE(as_uint) {
    String u16_string{ "6752" };
    auto   u16_or_none = u16_string.as_uint<u16>();
    VERIFY(u16_or_none.is_present());
    VERIFY_EQUAL(u16_or_none.value(), 6752);

    u16_string  = "-35";
    u16_or_none = u16_string.as_uint<u16>();
    VERIFY_FALSE(u16_or_none.is_present());

    String u64_string{ "8652164" };
    auto   u64_or_none = u64_string.as_uint<u64>();
    VERIFY(u64_or_none.is_present());
    VERIFY_EQUAL(u64_or_none.value(), 8652164);

    u64_string  = "00000657";
    u64_or_none = u64_string.as_uint<u64>();
    VERIFY(u64_or_none.is_present());
    VERIFY_EQUAL(u64_or_none.value(), 657);

    u64_string  = "56x7J9";
    u64_or_none = u64_string.as_uint<u64>();
    VERIFY_FALSE(u64_or_none.is_present());
}

TEST_CASE(as_uint_from_hex) {
    String string{ "0x1a2b3c" };
    auto   u32_or_none = string.as_uint_from_hex();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 0x1a2b3c);

    string      = "0f6d7a";
    u32_or_none = string.as_uint_from_hex();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 0x0f6d7a);

    string      = "i94hr";
    u32_or_none = string.as_uint_from_hex();
    VERIFY_FALSE(u32_or_none.is_present());

    string      = "0x";
    u32_or_none = string.as_uint_from_hex();
    VERIFY_FALSE(u32_or_none.is_present());
}

TEST_CASE(as_uint_from_octal) {
    String string{ "0612" };
    auto   u32_or_none = string.as_uint_from_octal();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 0612);

    string      = "35172";
    u32_or_none = string.as_uint_from_octal();
    VERIFY(u32_or_none.is_present());
    VERIFY_EQUAL(u32_or_none.value(), 035172);

    string      = "12634529";
    u32_or_none = string.as_uint_from_octal();
    VERIFY_FALSE(u32_or_none.is_present());
}

TEST_CASE(find) {
    String string{ "Hi everyone, this is a string" };

    auto index_or_none = string.find('i');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 1);

    index_or_none = string.find('a');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 21);

    index_or_none = string.find('a', 35);
    VERIFY_FALSE(index_or_none.is_present());

    index_or_none = string.find("this");
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 13);

    index_or_none = string.find("tr");
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 24);
}

TEST_CASE(find_last) {
    String string{ "Hi everyone, this is a string" };

    auto index_or_none = string.find_last('i');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 26);

    index_or_none = string.find_last('a');
    VERIFY(index_or_none.is_present());
    VERIFY_EQUAL(index_or_none.value(), 21);

    index_or_none = string.find_last('z');
    VERIFY_FALSE(index_or_none.is_present());
}

TEST_CASE(find_all) {
    String string{ "AbaCibbo Holy AbaCucca ly" };

    auto positions = string.find_all("Aba");
    VERIFY_FALSE(positions.is_empty());
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 0);
    VERIFY_EQUAL(positions.last(), 14);

    positions = string.find_all("xx");
    VERIFY(positions.is_empty());

    auto error_or_positions = string.try_find_all("ly");
    VERIFY(error_or_positions.is_value());

    positions = error_or_positions.unwrap_value();
    VERIFY_EQUAL(positions.count(), 2);
    VERIFY_EQUAL(positions.first(), 11);
    VERIFY_EQUAL(positions.last(), 23);
}

TEST_CASE(to_lowercase) {
    String string{ "HI TO EVERYONE" };

    auto error_or_lowercase_string = string.try_to_lowercase();
    VERIFY(error_or_lowercase_string.is_value());

    auto lowercase_string = error_or_lowercase_string.unwrap_value();
    VERIFY_EQUAL(lowercase_string, "hi to everyone");

    VERIFY_EQUAL(string.to_lowercase(), "hi to everyone");

    string = "Hi tO EvErYOnE";
    VERIFY_EQUAL(string.to_lowercase(), "hi to everyone");

    string = "Hi tO 6 % $ EvErYOnE 67";
    VERIFY_EQUAL(string.to_lowercase(), "hi to 6 % $ everyone 67");
}

TEST_CASE(to_uppercase) {
    String string{ "hi to everyone" };

    auto error_or_uppercase_string = string.try_to_uppercase();
    VERIFY(error_or_uppercase_string.is_value());

    auto lowercase_string = error_or_uppercase_string.unwrap_value();
    VERIFY_EQUAL(lowercase_string, "HI TO EVERYONE");

    VERIFY_EQUAL(string.to_uppercase(), "HI TO EVERYONE");

    string = "Hi tO EvErYOnE";
    VERIFY_EQUAL(string.to_uppercase(), "HI TO EVERYONE");

    string = "Hi tO 6 % $ EvErYOnE 67";
    VERIFY_EQUAL(string.to_uppercase(), "HI TO 6 % $ EVERYONE 67");
}

TEST_CASE(contains) {
    String string{ "Hi to everyone 1,2,3,4" };

    VERIFY(string.contains('1'));
    VERIFY(string.contains('e'));
    VERIFY(string.contains('E', CaseSensitivity::Insensitive));
    VERIFY_FALSE(string.contains('x'));
    VERIFY_FALSE(string.contains('E', CaseSensitivity::Sensitive));

    VERIFY(string.contains("to"));
    VERIFY(string.contains("2,3,4"));
    VERIFY(string.contains("eVeRyoNe", CaseSensitivity::Insensitive));
    VERIFY_FALSE(string.contains("not"));
    VERIFY_FALSE(string.contains("HI", CaseSensitivity::Sensitive));
}

TEST_CASE(iterator) {
    String string{ "0123456789" };

    char expected_value = '0';
    for ( auto c : string )
        VERIFY_EQUAL(c, expected_value++);
}