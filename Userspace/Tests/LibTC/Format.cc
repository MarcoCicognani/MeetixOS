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
#include <LibTC/Collection/List.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Text/Format.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

template<typename... Args>
void ensure_formatted(StringView expected_result, StringView format_view, Args&&... args) {
    StringBuilder string_builder{};

    auto error_or_void = format(string_builder, format_view, forward<Args>(args)...);
    VERIFY_FALSE(error_or_void.is_error());
    VERIFY_EQUAL(string_builder.as_string_view(), expected_result);
}

TEST_CASE(string_format) {
    ensure_formatted("Caught nullptr Exception at yz", "Caught {} {} at {}{}", nullptr, "Exception", 'y', 'z');
}

TEST_CASE(integral_format) {
    ensure_formatted("42", "{}", 42u);
    ensure_formatted("  42", "{:4}", 42u);
    ensure_formatted("00000042", "{:08}", 42u);
    ensure_formatted("-17", "{}", -17);
    ensure_formatted("    -17", "{:7}", -17);
    ensure_formatted("0013", "{:04}", 13);
    ensure_formatted("00001000", "{:08x}", 4096);
    ensure_formatted("1111222233334444", "{:x}", 0x1111222233334444uLL);
    ensure_formatted("12345678", "{:4}", 12345678);
}

TEST_CASE(escaped_braces) {
    ensure_formatted("{abc}", "{{{}}}", "abc");
}

TEST_CASE(mixed_formatting) {
    ensure_formatted("{0042/foo/      42/foo", "{{{:04}/{}/{:8}/{}", 42u, "foo", 42u, "foo");
}

TEST_CASE(appended_formatting) {
    StringBuilder string_builder{};

    auto error_or_void = format(string_builder, "Hi {}", "john");
    VERIFY_FALSE(error_or_void.is_error());

    error_or_void = format(string_builder, " I'm your {} friend", "best");
    VERIFY_FALSE(error_or_void.is_error());

    VERIFY_EQUAL(string_builder.as_string_view(), "Hi john I'm your best friend");
}

TEST_CASE(format_upper_case_integer) {
    ensure_formatted("  FF", "{:4X}", 0xff);
    ensure_formatted("0XFF", "{:#4X}", 0xff);
    ensure_formatted("11111111", "{:b}", 0xff);
    ensure_formatted("11111111", "{:B}", 0xff);
    ensure_formatted("0b11111111", "{:#b}", 0xff);
    ensure_formatted("0B11111111", "{:#B}", 0xff);
}

TEST_CASE(format_aligned) {
    ensure_formatted("13******", "{:*<8}", 13);
    ensure_formatted("***13***", "{:*^8}", 13);
    ensure_formatted("******13", "{:*>8}", 13);
    ensure_formatted("** 13***", "{:*^ 8}", 13);
}

TEST_CASE(format_octal) {
    ensure_formatted("744", "{:o}", 0744);
    ensure_formatted("0744", "{:#o}", 0744);
}

TEST_CASE(zero_pad) {
    ensure_formatted("42        ", "{: <010}", 42);
    ensure_formatted("0000000042", "{:010}", 42);
    ensure_formatted("////42////", "{:/^010}", 42);
    ensure_formatted("-0020", "{:04x}", -32);
    ensure_formatted("-0x000040", "{:#06x}", -64);
}

TEST_CASE(complex_string_specifiers) {
    ensure_formatted("12345678", "{:.8}", "123456789");
    ensure_formatted("abcd     ", "{:9}", "abcd");
    ensure_formatted("     abcd", "{:>9}", "abcd");
    ensure_formatted("  abcd   ", "{:^9}", "abcd");
    ensure_formatted("a   ", "{:4.6}", "a");
    ensure_formatted("abcdef", "{:4.6}", "abcdef");
    ensure_formatted("abcdef", "{:4.6}", "abcdefghi");
}

TEST_CASE(cast_integer_to_character) {
    ensure_formatted("a", "{:c}", static_cast<i32>('a'));
    ensure_formatted("f", "{:c}", static_cast<u32>('f'));
}

TEST_CASE(boolean_values) {
    ensure_formatted("true", "{}", true);
    ensure_formatted("false", "{}", false);
    ensure_formatted("true  ", "{:6}", true);
    ensure_formatted("false", "{:>4}", false);
    ensure_formatted("0", "{:d}", false);
    ensure_formatted("1", "{:d}", true);
    ensure_formatted("0x00000001", "{:#08x}", true);
}

TEST_CASE(pointers) {
    auto ptr = reinterpret_cast<void*>(0x4000);

    if ( sizeof(void*) == 4 ) {
        ensure_formatted("0x00000020", "{:p}", 32);
        ensure_formatted("0x00004000", "{:p}", ptr);
        ensure_formatted("0x00004000", "{}", ptr);
    } else if ( sizeof(void*) == 8 ) {
        ensure_formatted("0x0000000000000020", "{:p}", 32);
        ensure_formatted("0x0000000000004000", "{:p}", ptr);
        ensure_formatted("0x0000000000004000", "{}", ptr);
    } else
        VERIFY_NOT_REACHED();
}

TEST_CASE(floating_format) {
    ensure_formatted("1.12", "{}", 1.12);
    ensure_formatted("1", "{}", 1.);
    ensure_formatted("1.12", "{:.3}", 1.12);
    ensure_formatted("1.1", "{:.1}", 1.12);
    ensure_formatted("-1.12", "{}", -1.12);

    ensure_formatted("nan", "{}", NAN);
    ensure_formatted("inf", "{}", INFINITY);
    ensure_formatted("-inf", "{}", -INFINITY);

    ensure_formatted("xx1.1", "{:x>5.1}", 1.12);
}

TEST_CASE(no_precision_no_trailing_number) {
    ensure_formatted("0", "{:.0}", 0.1);
}

TEST_CASE(yay_this_implementation_sucks) {
    ensure_formatted("0", "{:.0}", 0.99999999999);
}

TEST_CASE(precision_with_trailing_zeros) {
    ensure_formatted("1.120", "{:0.3}", 1.12);
    ensure_formatted("1.1", "{:0.1}", 1.12);
}

TEST_CASE(magnitude_less_than_zero) {
    ensure_formatted("-0.654", "{}", -0.654);
    ensure_formatted("0.654", "{}", 0.654);
}

TEST_CASE(list_format) {
    ensure_formatted("[ 1, 2, 3, 4, 5, 6 ]", "{}", List{ 1, 2, 3, 4, 5, 6 });
}

TEST_CASE(pair_format) {
    ensure_formatted("hi: abc", "{}", Pair{ "hi"sv, "abc"sv });
    ensure_formatted("0x4: 0xff", "{:#x}", Pair{ 4, 0xff });
}

TEST_CASE(range_format) {
    StringBuilder string_builder{};

    ensure_formatted("0..25", "{}", Range{ 0, 25 });
    ensure_formatted("0..=25", "{}", RangeInclusive{ 0, 25 });
}

TEST_CASE(map_format) {
    StringBuilder string_builder{};

    ensure_formatted("{ z: 1, y: 2, x: 3 }", "{}", OrderedMap<StringView, usize>{ { "z", 1uL }, { "y", 2uL }, { "x", 3uL } });
}