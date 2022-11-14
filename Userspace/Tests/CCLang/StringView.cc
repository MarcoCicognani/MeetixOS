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

#include <CCLang/Alloc/String.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/StringView.hh>

#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(construction) {
    StringView empty_string_view{};
    verify$(empty_string_view.is_empty());
    verify$(empty_string_view.is_null());
    verify$(empty_string_view.is_null_or_empty());
    verify_equal$(empty_string_view, ""sv);

    auto const string                  = String::construct_from_view("Testing STC Strings"sv);
    auto const from_string_string_view = string.as_string_view();
    verify_false$(from_string_string_view.is_null_or_empty());
    verify_equal$(from_string_string_view.len(), 21);
    verify_equal$(from_string_string_view.len(), string.len());
    verify_equal$(from_string_string_view, "Testing STC Strings"sv);
    verify_equal$(from_string_string_view, String::construct_from_view("Testing STC Strings"sv));
}

TEST_CASE(assignment_operators) {
    StringView string_view{};
    verify$(string_view.is_null_or_empty());

    string_view = "Hello World"sv;
    verify_equal$(string_view.len(), 11);
    verify_equal$(string_view, "Hello World"sv);

    StringView string_view2{};
    string_view2 = string_view;
    verify_false$(string_view.is_null_or_empty());
    verify_false$(string_view2.is_null_or_empty());
    verify_equal$(string_view, "Hello World"sv);
    verify_equal$(string_view2, "Hello World"sv);

    StringView string_view3{};
    string_view3 = Cxx::move(string_view);
    verify$(string_view.is_null_or_empty());
    verify_equal$(string_view3.len(), 11);
    verify_false$(string_view3.is_null_or_empty());
    verify_equal$(string_view3, "Hello World"sv);
}

TEST_CASE(swap) {
    auto string_view  = "Hello everyone"sv;
    auto string_view2 = "Hi everyone"sv;

    verify_equal$(string_view, "Hello everyone"sv);
    verify_equal$(string_view2, "Hi everyone"sv);

    string_view.swap(string_view2);

    verify_equal$(string_view2, "Hello everyone"sv);
    verify_equal$(string_view, "Hi everyone"sv);
}

TEST_CASE(at_and_braces) {
    auto const string_view = "MeetixOS C++"sv;
    verify_equal$(string_view[0], 'M');
    verify_equal$(string_view[1], 'e');
    verify_equal$(string_view[2], 'e');
    verify_equal$(string_view[3], 't');
    verify_equal$(string_view[4], 'i');
    verify_equal$(string_view[5], 'x');
    verify_equal$(string_view[6], 'O');
    verify_equal$(string_view[7], 'S');
    verify_equal$(string_view[8], ' ');
    verify_equal$(string_view[9], 'C');
    verify_equal$(string_view[10], '+');
    verify_equal$(string_view[11], '+');

    verify_equal$(string_view.at(0), 'M');
    verify_equal$(string_view.at(1), 'e');
    verify_equal$(string_view.at(2), 'e');
    verify_equal$(string_view.at(3), 't');
    verify_equal$(string_view.at(4), 'i');
    verify_equal$(string_view.at(5), 'x');
    verify_equal$(string_view.at(6), 'O');
    verify_equal$(string_view.at(7), 'S');
    verify_equal$(string_view.at(8), ' ');
    verify_equal$(string_view.at(9), 'C');
    verify_equal$(string_view.at(10), '+');
    verify_equal$(string_view.at(11), '+');
}

TEST_CASE(compare) {
    auto const string_view = "Hello world OSDev"sv;

    verify_equal$(string_view.compare("Hello"sv), 1);
    verify_equal$(string_view.compare(""sv), 1);
    verify_equal$(string_view.compare("Hello world OSDev"sv), 0);
    verify_equal$(string_view.compare("Hello world OSDeveloper"sv), -1);

    auto const less_string_view  = "Hello"sv;
    auto const more_string_view  = "Hello world OSDeveloper"sv;
    auto const equal_string_view = "Hello World OSDev"sv;

    verify_less$(less_string_view, string_view);
    verify_greater$(more_string_view, string_view);
    verify_less_equal$(equal_string_view, string_view);
    verify_greater_equal$(equal_string_view, string_view);
}

TEST_CASE(equals_ignore_case) {
    auto const string_view = "Next generation Operating System"sv;

    verify$(string_view.equals_ignore_case("nExT GeneRAtIoN OpERatiNg SYStem"sv));
}

TEST_CASE(sub_string) {
    auto const string_view = "SplittableStringView"sv;

    verify_equal$(string_view.sub_string_view(10), "StringView"sv);
    verify_equal$(string_view.sub_string_view(3), "ittableStringView"sv);

    verify_equal$(string_view.sub_string_view(5, 3), "tab"sv);
    verify_equal$(string_view.sub_string_view(10, 6), "String"sv);
}

TEST_CASE(trim) {
    auto const string_view = "   \tYeah Buddy! \n \f \r"sv;

    verify_equal$(string_view.trim_whitespaces(), "Yeah Buddy!"sv);

    auto const string_view2 = "---Yeah Buddy+++"sv;
    verify_equal$(string_view2.trim("-+"sv, StringView::TrimMode::Left), "Yeah Buddy+++"sv);
    verify_equal$(string_view2.trim("-+"sv, StringView::TrimMode::Right), "---Yeah Buddy"sv);
    verify_equal$(string_view2.trim("-+"sv, StringView::TrimMode::Both), "Yeah Buddy"sv);
}

TEST_CASE(starts_with) {
    auto const string_view = "Hi I'm a StringView test"sv;

    verify$(string_view.starts_with('H'));
    verify$(string_view.starts_with('h', StringView::CaseSensible::Yes));
    verify$(string_view.starts_with("Hi"sv));
    verify$(string_view.starts_with("hi i'm a"sv, StringView::CaseSensible::Yes));
}

TEST_CASE(ends_with) {
    auto const string_view = "Beautiful world and beautiful country"sv;

    verify$(string_view.ends_with('y'));
    verify$(string_view.ends_with('Y', StringView::CaseSensible::No));
    verify$(string_view.ends_with("country"sv));
    verify$(string_view.ends_with("BeAuTifUl cOuNtRy"sv, StringView::CaseSensible::No));
}

TEST_CASE(as_int) {
    VERIFY_IS_VALUE_EQUAL("127"sv.as_int<i8>(), 127);
    VERIFY_IS_VALUE_EQUAL("-32"sv.as_int<i8>(), -32);
    VERIFY_IS_VALUE_EQUAL("450\n"sv.as_int(), 450);
    VERIFY_IS_VALUE_EQUAL("-65536"sv.as_int(), -65536);
    VERIFY_IS_VALUE_EQUAL("  \t64begin"sv.as_int(StringView::IntBase::Decimal, StringView::ParseMode::TrimWhitesAndBegin), 64);

    VERIFY_IS_VALUE_EQUAL("0xabc45"sv.as_int<i32>(StringView::IntBase::Hex), 0xabc45);
    VERIFY_IS_VALUE_EQUAL("-1ff24bc"sv.as_int<i32>(StringView::IntBase::Hex), -0x1ff24bc);

    VERIFY_IS_ERROR_EQUAL("\n 75"sv.as_int<i8>(StringView::IntBase::Decimal, StringView::ParseMode::BeginToEnd), EINVAL);
    VERIFY_IS_ERROR_EQUAL("7p5"sv.as_int<i8>(StringView::IntBase::Decimal), EINVAL);
    VERIFY_IS_ERROR_EQUAL("512"sv.as_int<i8>(StringView::IntBase::Decimal), EOVERFLOW);
}

TEST_CASE(as_uint) {
    VERIFY_IS_VALUE_EQUAL("9753"sv.as_uint<u16>(), 9753);
    VERIFY_IS_VALUE_EQUAL("6542037"sv.as_uint<u64>(), 6542037);
    VERIFY_IS_VALUE_EQUAL("000917"sv.as_uint<u64>(), 917);

    VERIFY_IS_ERROR_EQUAL("-64"sv.as_uint<u16>(), EINVAL);
    VERIFY_IS_ERROR_EQUAL("56AC"sv.as_uint<u64>(), EINVAL);
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
    verify_false$(positions.is_empty());
    verify_equal$(positions.count(), 2);
    verify_equal$(positions.first(), 0);
    verify_equal$(positions.last(), 14);

    positions = string_view.find_all("xx"sv);
    verify$(positions.is_empty());

    auto error_or_positions = string_view.try_find_all("ly"sv);
    verify$(error_or_positions.is_value());

    positions = error_or_positions.unwrap_value();
    verify_equal$(positions.count(), 2);
    verify_equal$(positions.first(), 11);
    verify_equal$(positions.last(), 23);
}

TEST_CASE(contains) {
    auto const string_view = "Hi to everyone 1,2,3,4"sv;

    verify$(string_view.contains('1'));
    verify$(string_view.contains('e'));
    verify$(string_view.contains('E', StringView::CaseSensible::No));
    verify_false$(string_view.contains('x'));
    verify_false$(string_view.contains('E', StringView::CaseSensible::Yes));

    verify$(string_view.contains("to"sv));
    verify$(string_view.contains("2,3,4"sv));
    verify$(string_view.contains("eVeRyoNe"sv, StringView::CaseSensible::No));
    verify_false$(string_view.contains("not"sv));
    verify_false$(string_view.contains("HI"sv, StringView::CaseSensible::Yes));
}

TEST_CASE(iterator) {
    auto const string_view = "0123456789"sv;

    char i = '0';
    for ( auto const c : string_view )
        verify_equal$(c, i++);
}

TEST_CASE(reverse_iterator) {
    auto const string_view = "9876543210"sv;

    char i = '0';
    for ( auto const c : string_view.reverse_iter() )
        verify_equal$(c, i++);

    verify_equal$(i, '9' + 1);
}