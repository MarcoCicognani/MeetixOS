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

#include <LibMath/math.h>
#include <CCLang/Alloc/List.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Alloc/Text/Format.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

template<typename... Args>
void ensure_formatted(StringView expected_result, StringView format_view, Args... args) {
    auto string_builder = StringBuilder::construct_empty();

    VERIFY_IS_VALUE(format(string_builder, format_view, Cxx::forward<Args>(args)...));
    verify_equal$(string_builder.as_string_view(), expected_result);
}

TEST_CASE(string_format) {
    ensure_formatted("Caught nullptr Exception at yz"sv, "Caught {} {} at {}{}"sv, nullptr, "Exception"sv, 'y', 'z');
}

TEST_CASE(integral_format) {
    ensure_formatted("42"sv, "{}"sv, 42u);
    ensure_formatted("  42"sv, "{:4}"sv, 42u);
    ensure_formatted("00000042"sv, "{:08}"sv, 42u);
    ensure_formatted("-17"sv, "{}"sv, -17);
    ensure_formatted("    -17"sv, "{:7}"sv, -17);
    ensure_formatted("0013"sv, "{:04}"sv, 13);
    ensure_formatted("00001000"sv, "{:08x}"sv, 4096);
    ensure_formatted("1111222233334444"sv, "{:x}"sv, 0x1111222233334444uLL);
    ensure_formatted("12345678"sv, "{:4}"sv, 12345678);
}

TEST_CASE(escaped_braces) {
    ensure_formatted("{abc}"sv, "{{{}}}"sv, "abc"sv);
}

TEST_CASE(mixed_formatting) {
    ensure_formatted("{0042/foo/      42/foo"sv, "{{{:04}/{}/{:8}/{}"sv, 42u, "foo"sv, 42u, "foo"sv);
}

TEST_CASE(appended_formatting) {
    auto string_builder = StringBuilder::construct_empty();

    VERIFY_IS_VALUE(format(string_builder, "Hi {}"sv, "john"sv));
    VERIFY_IS_VALUE(format(string_builder, " I'm your {} friend"sv, "best"sv));

    verify_equal$(string_builder.as_string_view(), "Hi john I'm your best friend"sv);
}

TEST_CASE(format_upper_case_integer) {
    ensure_formatted("  FF"sv, "{:4X}"sv, 0xff);
    ensure_formatted("0XFF"sv, "{:#4X}"sv, 0xff);
    ensure_formatted("11111111"sv, "{:b}"sv, 0xff);
    ensure_formatted("11111111"sv, "{:B}"sv, 0xff);
    ensure_formatted("0b11111111"sv, "{:#b}"sv, 0xff);
    ensure_formatted("0B11111111"sv, "{:#B}"sv, 0xff);
}

TEST_CASE(format_aligned) {
    ensure_formatted("13******"sv, "{:*<8}"sv, 13);
    ensure_formatted("***13***"sv, "{:*^8}"sv, 13);
    ensure_formatted("******13"sv, "{:*>8}"sv, 13);
    ensure_formatted("** 13***"sv, "{:*^ 8}"sv, 13);
}

TEST_CASE(format_octal) {
    ensure_formatted("744"sv, "{:o}"sv, 0744);
    ensure_formatted("0744"sv, "{:#o}"sv, 0744);
}

TEST_CASE(zero_pad) {
    ensure_formatted("42        "sv, "{: <010}"sv, 42);
    ensure_formatted("0000000042"sv, "{:010}"sv, 42);
    ensure_formatted("////42////"sv, "{:/^010}"sv, 42);
    ensure_formatted("-0020"sv, "{:04x}"sv, -32);
    ensure_formatted("-0x000040"sv, "{:#06x}"sv, -64);
}

TEST_CASE(complex_string_specifiers) {
    ensure_formatted("12345678"sv, "{:.8}"sv, "123456789"sv);
    ensure_formatted("abcd     "sv, "{:9}"sv, "abcd"sv);
    ensure_formatted("     abcd"sv, "{:>9}"sv, "abcd"sv);
    ensure_formatted("  abcd   "sv, "{:^9}"sv, "abcd"sv);
    ensure_formatted("a   "sv, "{:4.6}"sv, "a"sv);
    ensure_formatted("abcdef"sv, "{:4.6}"sv, "abcdef"sv);
    ensure_formatted("abcdef"sv, "{:4.6}"sv, "abcdefghi"sv);
}

TEST_CASE(cast_integer_to_character) {
    ensure_formatted("a"sv, "{:c}"sv, static_cast<i32>('a'));
    ensure_formatted("f"sv, "{:c}"sv, static_cast<u32>('f'));
}

TEST_CASE(boolean_values) {
    ensure_formatted("true"sv, "{}"sv, true);
    ensure_formatted("false"sv, "{}"sv, false);
    ensure_formatted("true  "sv, "{:6}"sv, true);
    ensure_formatted("false"sv, "{:>4}"sv, false);
    ensure_formatted("0"sv, "{:d}"sv, false);
    ensure_formatted("1"sv, "{:d}"sv, true);
    ensure_formatted("0x00000001"sv, "{:#08x}"sv, true);
}

TEST_CASE(pointers) {
    auto ptr = reinterpret_cast<void*>(0x4000);

    if ( sizeof(void*) == 4 ) {
        ensure_formatted("0x00000020"sv, "{:p}"sv, 32);
        ensure_formatted("0x00004000"sv, "{:p}"sv, ptr);
        ensure_formatted("0x00004000"sv, "{}"sv, ptr);
    } else if ( sizeof(void*) == 8 ) {
        ensure_formatted("0x0000000000000020"sv, "{:p}"sv, 32);
        ensure_formatted("0x0000000000004000"sv, "{:p}"sv, ptr);
        ensure_formatted("0x0000000000004000"sv, "{}"sv, ptr);
    } else
        verify_not_reached$();
}

TEST_CASE(floating_format) {
    ensure_formatted("1.12"sv, "{}"sv, 1.12);
    ensure_formatted("1"sv, "{}"sv, 1.);
    ensure_formatted("1.12"sv, "{:.3}"sv, 1.12);
    ensure_formatted("1.1"sv, "{:.1}"sv, 1.12);
    ensure_formatted("-1.12"sv, "{}"sv, -1.12);

    ensure_formatted("nan"sv, "{}"sv, NAN);
    ensure_formatted("inf"sv, "{}"sv, INFINITY);
    ensure_formatted("-inf"sv, "{}"sv, -INFINITY);

    ensure_formatted("xx1.1"sv, "{:x>5.1}"sv, 1.12);
}

TEST_CASE(no_precision_no_trailing_number) {
    ensure_formatted("0"sv, "{:.0}"sv, 0.1);
}

TEST_CASE(yay_this_implementation_sucks) {
    ensure_formatted("0"sv, "{:.0}"sv, 0.99999999999);
}

TEST_CASE(precision_with_trailing_zeros) {
    ensure_formatted("1.120"sv, "{:0.3}"sv, 1.12);
    ensure_formatted("1.1"sv, "{:0.1}"sv, 1.12);
}

TEST_CASE(magnitude_less_than_zero) {
    ensure_formatted("-0.654"sv, "{}"sv, -0.654);
    ensure_formatted("0.654"sv, "{}"sv, 0.654);
}

TEST_CASE(list_format) {
    auto const list = List<i32>::construct_from_list({ 1, 2, 3, 4, 5, 6 });
    ensure_formatted("[ 1, 2, 3, 4, 5, 6 ]"sv, "{}"sv, list.clone());
}

TEST_CASE(range_format) {
    ensure_formatted("0..25"sv, "{}"sv, Range{ 0, 25 });
    ensure_formatted("0..=25"sv, "{}"sv, RangeInclusive{ 0, 25 });
}

TEST_CASE(map_format) {
    auto const unordered_map = OrderedMap<StringView, usize>::construct_from_list({ { "z"sv, 1uL }, { "y"sv, 2uL }, { "x"sv, 3uL } });
    ensure_formatted("{ z: 1, y: 2, x: 3 }"sv, "{}"sv, unordered_map.clone());
}