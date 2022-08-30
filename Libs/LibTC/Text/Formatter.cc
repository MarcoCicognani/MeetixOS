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

#include <LibTC/Assertions.hh>
#include <LibTC/BitCast.hh>
#include <LibTC/Collection/Range.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Math.hh>
#include <LibTC/NumericLimits.hh>
#include <LibTC/Text/Format.hh>
#include <LibTC/Text/Formatter.hh>

#ifndef IN_KERNEL
#    include <LibMath/math.h>
#endif

namespace TC::Text {

auto FormatApplier::construct_from_string_builder(StringBuilder& string_builder) -> FormatApplier {
    return FormatApplier{ string_builder, FormatParser::Result{} };
}

auto FormatApplier::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> FormatApplier {
    return FormatApplier{ string_builder, Cxx::move(result) };
}

FormatApplier::FormatApplier(FormatApplier&& rhs)
    : m_string_builder{ rhs.m_string_builder }
    , m_parser_result{ Cxx::move(rhs.m_parser_result) } {
}

auto FormatApplier::operator=(FormatApplier&& rhs) -> FormatApplier& {
    FormatApplier format_applier{ Cxx::move(rhs) };
    swap(format_applier);
    return *this;
}

auto FormatApplier::clone_format_applier() const -> FormatApplier {
    return FormatApplier{ m_string_builder, m_parser_result };
}

auto FormatApplier::swap(FormatApplier& rhs) -> void {
    Cxx::swap(m_string_builder, rhs.m_string_builder);
    Cxx::swap(m_parser_result, rhs.m_parser_result);
}

auto FormatApplier::try_put_padding(char fill, usize amount) -> ErrorOr<void> {
    for ( [[maybe_unused]] usize i : Range{ 0u, amount } )
        TRY(m_string_builder.try_append(fill));
    return {};
}

auto FormatApplier::try_put_literal(StringView literals_view) -> ErrorOr<void> {
    usize i = 0;
    while ( i < literals_view.len() ) {
        TRY(m_string_builder.try_append(literals_view[i]));

        /* skip escaped placeholders */
        auto const sub_string_view = literals_view.sub_string_view(i);
        if ( sub_string_view.starts_with("{{"sv) || sub_string_view.starts_with("}}"sv) )
            i += 2;
        else
            ++i;
    }
    return {};
}

auto FormatApplier::try_put_string(StringView value, usize min_width, usize max_width, FormatParser::Alignment alignment, char alignment_fill)
    -> ErrorOr<void> {
    /* ensure the alignment */
    if ( alignment == FormatParser::Alignment::Default )
        alignment = FormatParser::Alignment::Left;

    auto value_width   = min(max_width, value.len());
    auto padding_width = max(min_width, value_width) - value_width;

    /* truncate the string if the precision is less than his length */
    if ( value_width < value.len() )
        value = value.sub_string_view(0, value_width);

    /* put the value into the output string builder with the padding */
    switch ( alignment ) {
        case FormatParser::Alignment::Left:
            TRY(m_string_builder.try_append(value));
            TRY(try_put_padding(alignment_fill, padding_width));
            break;
        case FormatParser::Alignment::Center: {
            auto left_padding_width  = padding_width / 2;
            auto right_padding_width = ceil_div<usize, usize>(padding_width, 2);

            TRY(try_put_padding(alignment_fill, left_padding_width));
            TRY(m_string_builder.try_append(value));
            TRY(try_put_padding(alignment_fill, right_padding_width));
            break;
        }
        case FormatParser::Alignment::Right:
            TRY(try_put_padding(alignment_fill, padding_width));
            TRY(m_string_builder.try_append(value));
            break;
        default:
            VERIFY_NOT_REACHED();
    }
    return {};
}

auto FormatApplier::try_put_u64(u64                           value,
                                u8                            base,
                                FormatParser::ShowBase        show_base_prefix,
                                bool                          upper_case,
                                FormatParser::ZeroPad         zero_pad,
                                usize                         min_width,
                                FormatParser::Alignment       alignment,
                                char                          alignment_fill,
                                FormatParser::ShowIntegerSign integer_sign,
                                bool                          is_negative) -> ErrorOr<void> {
    /* ensure the alignment */
    if ( alignment == FormatParser::Alignment::Default )
        alignment = FormatParser::Alignment::Right;

    /* convert the integer value to a string */
    char to_char_buffer[128];
    auto digits_width = convert_unsigned_to_chars(value, to_char_buffer, base, upper_case);

    /* calculate the width */
    usize prefix_width;
    if ( alignment == FormatParser::Alignment::Right && zero_pad == FormatParser::ZeroPad::Yes ) {
        /* To ensure format("{:#08x}", 32) produces '0x00000020' instead '0x000020' */
        prefix_width = 0;
    } else {
        if ( is_negative || integer_sign != FormatParser::ShowIntegerSign::IfNegative )
            prefix_width = 1;
        else
            prefix_width = 0;

        /* prepare the width */
        if ( show_base_prefix == FormatParser::ShowBase::Yes ) {
            switch ( base ) {
                case 2:
                case 16:
                    prefix_width += 2;
                    break;
                case 8:
                    prefix_width += 1;
                    break;
                default:
                    VERIFY_NOT_REACHED();
            }
        }
    }

    auto field_width   = prefix_width + digits_width;
    auto padding_width = max(field_width, min_width) - field_width;

    auto try_put_prefix = [&]() -> ErrorOr<void> {
        /* prepend the sign or the space if necessary */
        if ( is_negative )
            TRY(m_string_builder.try_append('-'));
        else if ( integer_sign == FormatParser::ShowIntegerSign::Yes )
            TRY(m_string_builder.try_append('+'));
        else if ( integer_sign == FormatParser::ShowIntegerSign::KeepSpace )
            TRY(m_string_builder.try_append(' '));

        /* put the base prefix if requested */
        if ( show_base_prefix == FormatParser::ShowBase::Yes ) {
            switch ( base ) {
                case 2:
                    if ( upper_case )
                        TRY(m_string_builder.try_append("0B"sv));
                    else
                        TRY(m_string_builder.try_append("0b"sv));
                    break;
                case 8:
                    TRY(m_string_builder.try_append('0'));
                    break;
                case 16:
                    if ( upper_case )
                        TRY(m_string_builder.try_append("0X"sv));
                    else
                        TRY(m_string_builder.try_append("0x"sv));
                    break;
                default:
                    VERIFY_NOT_REACHED();
            }
        }
        return {};
    };
    auto try_put_digits = [&]() -> ErrorOr<void> {
        for ( usize i : Range{ 0u, digits_width } )
            TRY(m_string_builder.try_append(to_char_buffer[i]));
        return {};
    };

    /* put the value into the output string builder with the padding */
    switch ( alignment ) {
        case FormatParser::Alignment::Left:
            TRY(try_put_prefix());
            TRY(try_put_digits());
            TRY(try_put_padding(alignment_fill, padding_width));
            break;
        case FormatParser::Alignment::Center: {
            auto left_padding_width  = padding_width / 2;
            auto right_padding_width = ceil_div<usize, usize>(padding_width, 2);

            TRY(try_put_padding(alignment_fill, left_padding_width));
            TRY(try_put_prefix());
            TRY(try_put_digits());
            TRY(try_put_padding(alignment_fill, right_padding_width));
            break;
        }
        case FormatParser::Alignment::Right:
            if ( zero_pad == FormatParser::ZeroPad::Yes ) {
                TRY(try_put_prefix());
                TRY(try_put_padding('0', padding_width));
                TRY(try_put_digits());
            } else {
                TRY(try_put_padding(alignment_fill, padding_width));
                TRY(try_put_prefix());
                TRY(try_put_digits());
            }
            break;
        default:
            VERIFY_NOT_REACHED();
    }

    return {};
}

auto FormatApplier::try_put_i64(i64                           value,
                                u8                            base,
                                FormatParser::ShowBase        show_base_prefix,
                                bool                          upper_case,
                                FormatParser::ZeroPad         zero_pad,
                                usize                         min_width,
                                FormatParser::Alignment       alignment,
                                char                          alignment_fill,
                                FormatParser::ShowIntegerSign integer_sign) -> ErrorOr<void> {
    /* module of the value */
    bool is_negative = value < 0;
    TRY(try_put_u64(static_cast<u64>(is_negative ? -value : value),
                    base,
                    show_base_prefix,
                    upper_case,
                    zero_pad,
                    min_width,
                    alignment,
                    alignment_fill,
                    integer_sign,
                    is_negative));

    return {};
}

#ifndef IN_KERNEL
auto FormatApplier::try_put_f64(double                        value,
                                u8                            base,
                                bool                          upper_case,
                                FormatParser::ZeroPad         zero_pad,
                                FormatParser::Alignment       alignment,
                                usize                         min_width,
                                usize                         precision,
                                char                          alignment_fill,
                                FormatParser::ShowIntegerSign integer_sign) -> ErrorOr<void> {
    auto string_builder = StringBuilder::construct_empty();
    auto format_applier = FormatApplier::construct_from_string_builder(string_builder);

    /* write-out the NotANumber and Infinite values */
    if ( isnan(value) || isinf(value) ) [[unlikely]] {
        if ( value < 0.0 )
            TRY(string_builder.try_append('-'));
        else if ( integer_sign == FormatParser::ShowIntegerSign::Yes )
            TRY(string_builder.try_append('+'));
        else if ( integer_sign == FormatParser::ShowIntegerSign::KeepSpace )
            TRY(string_builder.try_append(' '));

        if ( isnan(value) )
            TRY(string_builder.try_append(upper_case ? "NAN"sv : "nan"sv));
        else
            TRY(string_builder.try_append(upper_case ? "INF"sv : "inf"sv));

        TRY(try_put_string(string_builder.as_string_view(), min_width, 0xfffffff, alignment, alignment_fill));
        return {};
    }

    /* check for negative value and take the module */
    bool is_negative = value < 0.0;
    if ( is_negative )
        value = -value;

    /* put out the integer part */
    TRY(format_applier.try_put_u64(static_cast<u64>(value),
                                   base,
                                   FormatParser::ShowBase::No,
                                   upper_case,
                                   FormatParser::ZeroPad::No,
                                   0,
                                   FormatParser::Alignment::Default,
                                   ' ',
                                   integer_sign,
                                   is_negative));

    /* approximate the decimal part */
    if ( precision > 0 ) {
        value -= static_cast<i64>(value);

        /* make the epsilon precision value */
        double epsilon = 0.5;
        for ( [[maybe_unused]] usize i : Range{ 0u, precision } )
            epsilon /= 10.0;

        /* calculate the visible precision chars */
        usize visible_precision = 0;
        for ( ; visible_precision < precision; ++visible_precision ) {
            if ( value - static_cast<i64>(value) < epsilon )
                break;

            value *= 10.0;
            epsilon *= 10.0;
        }

        if ( zero_pad == FormatParser::ZeroPad::Yes || visible_precision > 0 )
            TRY(string_builder.try_append('.'));
        if ( visible_precision > 0 ) {
            TRY(format_applier
                    .try_put_u64(static_cast<u64>(value), base, FormatParser::ShowBase::No, upper_case, FormatParser::ZeroPad::Yes, visible_precision));
        }
        if ( zero_pad == FormatParser::ZeroPad::Yes && (precision - visible_precision) > 0 ) {
            TRY(format_applier.try_put_u64(0, base, FormatParser::ShowBase::No, false, FormatParser::ZeroPad::Yes, precision - visible_precision));
        }
    }

    /* put into this builder the values */
    TRY(try_put_string(string_builder.as_string_view(), min_width, 0xffffff, alignment, alignment_fill));
    return {};
}

auto FormatApplier::try_put_f80(long double                   value,
                                u8                            base,
                                bool                          upper_case,
                                FormatParser::Alignment       alignment,
                                usize                         min_width,
                                usize                         precision,
                                char                          alignment_fill,
                                FormatParser::ShowIntegerSign integer_sign) -> ErrorOr<void> {
    auto string_builder = StringBuilder::construct_empty();
    auto format_applier = FormatApplier::construct_from_string_builder(string_builder);

    /* write-out the NotANumber and Infinite values */
    if ( isnan(value) || isinf(value) ) [[unlikely]] {
        if ( value < 0.0L )
            TRY(string_builder.try_append('-'));
        else if ( integer_sign == FormatParser::ShowIntegerSign::Yes )
            TRY(string_builder.try_append('+'));
        else if ( integer_sign == FormatParser::ShowIntegerSign::KeepSpace )
            TRY(string_builder.try_append(' '));

        if ( isnan(value) )
            TRY(string_builder.try_append(upper_case ? "NAN"sv : "nan"sv));
        else
            TRY(string_builder.try_append(upper_case ? "INF"sv : "inf"sv));

        TRY(try_put_string(string_builder.as_string_view(), min_width, 0xfffffff, alignment, alignment_fill));
        return {};
    }

    /* check for negative value and take the module */
    bool is_negative = value < 0.0L;
    if ( is_negative )
        value = -value;

    /* put out the integer part */
    TRY(format_applier.try_put_u64(static_cast<u64>(value),
                                   base,
                                   FormatParser::ShowBase::No,
                                   upper_case,
                                   FormatParser::ZeroPad::Yes,
                                   0,
                                   FormatParser::Alignment::Default,
                                   ' ',
                                   integer_sign,
                                   is_negative));

    /* approximate the decimal part */
    if ( precision > 0 ) {
        value -= static_cast<i64>(value);

        /* make the epsilon precision value */
        long double epsilon = 0.5L;
        for ( [[maybe_unused]] usize i : Range{ 0u, precision } )
            epsilon /= 10.0L;

        /* calculate the visible precision chars */
        usize visible_precision = 0;
        for ( ; visible_precision < precision; ++visible_precision ) {
            if ( value - static_cast<i64>(value) < epsilon )
                break;

            value *= 10.0L;
            epsilon *= 10.0L;
        }

        if ( visible_precision > 0 ) {
            TRY(string_builder.try_append('.'));
            TRY(format_applier
                    .try_put_u64(static_cast<u64>(value), base, FormatParser::ShowBase::No, upper_case, FormatParser::ZeroPad::Yes, visible_precision));
        }
    }

    /* put into this builder the values */
    TRY(try_put_string(string_builder.as_string_view(), min_width, 0xffffff, alignment, alignment_fill));
    return {};
}
#endif

auto FormatApplier::string_builder() -> StringBuilder& {
    return m_string_builder;
}

auto FormatApplier::alignment_fill() const -> char {
    return m_parser_result.m_alignment_fill;
}

auto FormatApplier::alignment() const -> FormatParser::Alignment {
    return m_parser_result.m_alignment;
}

auto FormatApplier::show_integer_sign() const -> FormatParser::ShowIntegerSign {
    return m_parser_result.m_show_integer_sign;
}

auto FormatApplier::show_base() const -> FormatParser::ShowBase {
    return m_parser_result.m_show_base;
}

auto FormatApplier::zero_pad() const -> FormatParser::ZeroPad {
    return m_parser_result.m_zero_pad;
}

auto FormatApplier::width() const -> Option<usize> {
    return m_parser_result.m_width;
}

auto FormatApplier::precision() const -> Option<usize> {
    return m_parser_result.m_precision;
}

auto FormatApplier::display_as() const -> FormatParser::DisplayAs {
    return m_parser_result.m_display_as;
}

auto FormatApplier::display_as_is_numeric() const -> bool {
    return m_parser_result.display_as_is_numeric();
}

auto FormatApplier::set_alignment_fill(char alignment_fill) -> void {
    m_parser_result.m_alignment_fill = alignment_fill;
}

auto FormatApplier::set_alignment(FormatParser::Alignment alignment) -> void {
    m_parser_result.m_alignment = alignment;
}

auto FormatApplier::set_show_integer_sign(FormatParser::ShowIntegerSign show_integer_sign) -> void {
    m_parser_result.m_show_integer_sign = show_integer_sign;
}

auto FormatApplier::set_show_base(FormatParser::ShowBase show_base) -> void {
    m_parser_result.m_show_base = show_base;
}

auto FormatApplier::set_zero_pad(FormatParser::ZeroPad zero_pad) -> void {
    m_parser_result.m_zero_pad = zero_pad;
}

auto FormatApplier::set_width(Option<usize> width) -> void {
    m_parser_result.m_width = width;
}

auto FormatApplier::set_precision(Option<usize> precision) -> void {
    m_parser_result.m_precision = precision;
}

auto FormatApplier::set_display_as(FormatParser::DisplayAs display_as) -> void {
    m_parser_result.m_display_as = display_as;
}

FormatApplier::FormatApplier(StringBuilder& string_builder, FormatParser::Result result)
    : m_string_builder{ string_builder }
    , m_parser_result{ Cxx::move(result) } {
}

auto FormatApplier::convert_unsigned_to_chars(u64 value, char to_chars_buffer[128], u8 base, bool upper_case) -> usize {
    VERIFY_GREATER_EQUAL(base, 2);
    VERIFY_LESS_EQUAL(base, 16);

    /* short way for 0 value */
    if ( value == 0 ) {
        to_chars_buffer[0] = '0';
        return 1;
    }

    /* convert the digits */
    usize       used_chars  = 0;
    char const* char_digits = upper_case ? "0123456789ABCDEF" : "0123456789abcdef";
    while ( value > 0 ) {
        to_chars_buffer[used_chars++] = char_digits[value % base];

        value /= base;
    }

    /* flip the buffer */
    for ( usize i : Range{ 0u, used_chars / 2 } )
        Cxx::swap(to_chars_buffer[i], to_chars_buffer[used_chars - i - 1]);

    return used_chars;
}

auto Formatter<nullptr_t>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<nullptr_t> {
    return Formatter<nullptr_t>{ Cxx::move(format_applier) };
}

auto Formatter<nullptr_t>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<nullptr_t> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<nullptr_t>::format(nullptr_t) -> ErrorOr<void> {
    TRY(try_put_string("nullptr"sv,
                       width().value_or(NumericLimits<usize>::min()),
                       precision().value_or(NumericLimits<usize>::max()),
                       alignment(),
                       alignment_fill()));
    return {};
}

Formatter<nullptr_t>::Formatter(FormatApplier format_applier)
    : FormatApplier{ Cxx::move(format_applier) } {
}

auto Formatter<StringView>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<StringView> {
    return Formatter<StringView>{ Cxx::move(format_applier) };
}

auto Formatter<StringView>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<StringView> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<StringView>::format(StringView value) -> ErrorOr<void> {
    if ( show_integer_sign() != FormatParser::ShowIntegerSign::IfNegative )
        VERIFY_NOT_REACHED();
    if ( show_base() != FormatParser::ShowBase::No )
        VERIFY_NOT_REACHED();
    if ( zero_pad() != FormatParser::ZeroPad::No )
        VERIFY_NOT_REACHED();
    if ( display_as() != FormatParser::DisplayAs::Default && display_as() != FormatParser::DisplayAs::String && display_as() != FormatParser::DisplayAs::Char )
        VERIFY_NOT_REACHED();

    TRY(try_put_string(value, width().value_or(NumericLimits<usize>::min()), precision().value_or(NumericLimits<usize>::max()), alignment(), alignment_fill()));
    return {};
}

Formatter<StringView>::Formatter(FormatApplier format_applier)
    : FormatApplier{ Cxx::move(format_applier) } {
}

template<Integral T>
auto Formatter<T>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<T> {
    return Formatter<T>{ Cxx::move(format_applier) };
}

template<Integral T>
auto Formatter<T>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<T> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

template<Integral T>
auto Formatter<T>::format(T value) -> ErrorOr<void> {
    if ( display_as() == FormatParser::DisplayAs::Char ) {
        VERIFY_GREATER_EQUAL(value, 0);
        VERIFY_LESS_EQUAL(value, 127);

        /* display as string */
        set_display_as(FormatParser::DisplayAs::String);

        /* forward to the string-view formatter */
        auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
        TRY(sv_formatter.format(StringView{ bit_cast<char const*>(&value), 1 }));

        return {};
    }

    /* integral format does not support precision */
    if ( precision().is_present() )
        VERIFY_NOT_REACHED();

    /* prepare for showing as pointer */
    if ( display_as() == FormatParser::DisplayAs::Pointer ) {
        if ( show_integer_sign() != FormatParser::ShowIntegerSign::IfNegative )
            VERIFY_NOT_REACHED();
        if ( alignment() != FormatParser::Alignment::Default )
            VERIFY_NOT_REACHED();
        if ( width().is_present() )
            VERIFY_NOT_REACHED();

        /* set specifications for pointer formatting */
        set_display_as(FormatParser::DisplayAs::Hex);
        set_show_base(FormatParser::ShowBase::Yes);
        set_zero_pad(FormatParser::ZeroPad::Yes);
        set_width(sizeof(void*) * 2);
    }

    u8   base       = 0;
    bool upper_case = false;
    switch ( display_as() ) {
        case FormatParser::DisplayAs::Default:
        case FormatParser::DisplayAs::Decimal:
            base = 10;
            break;
        case FormatParser::DisplayAs::Binary:
            base = 2;
            break;
        case FormatParser::DisplayAs::BinaryUpperCase:
            base       = 2;
            upper_case = true;
            break;
        case FormatParser::DisplayAs::Octal:
            base = 8;
            break;
        case FormatParser::DisplayAs::Hex:
            base = 16;
            break;
        case FormatParser::DisplayAs::HexUpperCase:
            base       = 16;
            upper_case = true;
            break;
        default:
            VERIFY_NOT_REACHED();
    }

    /* put the number into the string-builder */
    if constexpr ( IsSame<MakeUnsigned<T>, T> ) {
        TRY(try_put_u64(value,
                        base,
                        show_base(),
                        upper_case,
                        zero_pad(),
                        width().value_or(NumericLimits<usize>::min()),
                        alignment(),
                        alignment_fill(),
                        show_integer_sign(),
                        false));
    } else {
        TRY(try_put_i64(value,
                        base,
                        show_base(),
                        upper_case,
                        zero_pad(),
                        width().value_or(NumericLimits<usize>::min()),
                        alignment(),
                        alignment_fill(),
                        show_integer_sign()));
    }

    return {};
}

template<Integral T>
Formatter<T>::Formatter(FormatApplier format_applier)
    : FormatApplier{ Cxx::move(format_applier) } {
}

template class Formatter<u8>;
template class Formatter<u16>;
template class Formatter<u32>;
template class Formatter<u64>;

template class Formatter<i8>;
template class Formatter<i16>;
template class Formatter<i32>;
template class Formatter<i64>;

auto Formatter<bool>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<bool> {
    return Formatter<bool>{ Cxx::move(format_applier) };
}

auto Formatter<bool>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<bool> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<bool>::format(bool value) const -> ErrorOr<void> {
    if ( display_as_is_numeric() ) {
        auto u8_formatter = Formatter<u8>::construct_from_format_applier(clone_format_applier());
        TRY(u8_formatter.format(static_cast<u8>(value)));
    } else {
        auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
        TRY(sv_formatter.format(value ? "true"sv : "false"sv));
    }

    return {};
}

Formatter<bool>::Formatter(FormatApplier format_applier)
    : FormatApplier{ Cxx::move(format_applier) } {
}

auto Formatter<char>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<char> {
    return Formatter<char>{ Cxx::move(format_applier) };
}

auto Formatter<char>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<char> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<char>::format(char value) const -> ErrorOr<void> {
    if ( display_as_is_numeric() ) {
        auto i8_formatter = Formatter<i8>::construct_from_format_applier(clone_format_applier());
        TRY(i8_formatter.format(value));
    } else {
        auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
        TRY(sv_formatter.format(StringView{ &value, 1 }));
    }

    return {};
}

Formatter<char>::Formatter(FormatApplier format_applier)
    : FormatApplier{ Cxx::move(format_applier) } {
}

#ifndef IN_KERNEL
auto Formatter<float>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<float> {
    return Formatter<float>{ Cxx::move(format_applier) };
}

auto Formatter<float>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<float> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<float>::format(float value) const -> ErrorOr<void> {
    auto double_formatter = Formatter<double>::construct_from_format_applier(clone_format_applier());
    TRY(double_formatter.format(static_cast<double>(value)));

    return {};
}

Formatter<float>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<double>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<double> {
    return Formatter<double>{ Cxx::move(format_applier) };
}

auto Formatter<double>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<double> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<double>::format(double value) -> ErrorOr<void> {
    /* prepare the base and the case */
    u8   base       = 0;
    bool upper_case = false;
    switch ( display_as() ) {
        case FormatParser::DisplayAs::Default:
        case FormatParser::DisplayAs::Float:
            base = 10;
            break;
        case FormatParser::DisplayAs::HexFloat:
            base = 16;
            break;
        case FormatParser::DisplayAs::HexFloatUpperCase:
            base       = 16;
            upper_case = true;
            break;
        default:
            VERIFY_NOT_REACHED();
    }

    /* put the double value into the string-builder */
    TRY(try_put_f64(value,
                    base,
                    upper_case,
                    zero_pad(),
                    alignment(),
                    width().value_or(NumericLimits<usize>::min()),
                    precision().value_or(6),
                    alignment_fill(),
                    show_integer_sign()));
    return {};
}

Formatter<double>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<long double>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<long double> {
    return Formatter<long double>{ Cxx::move(format_applier) };
}

auto Formatter<long double>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<long double> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<long double>::format(long double value) -> ErrorOr<void> {
    /* prepare the base and the case */
    u8   base       = 0;
    bool upper_case = false;
    switch ( display_as() ) {
        case FormatParser::DisplayAs::Default:
        case FormatParser::DisplayAs::Float:
            base = 10;
            break;
        case FormatParser::DisplayAs::HexFloat:
            base = 16;
            break;
        case FormatParser::DisplayAs::HexFloatUpperCase:
            base       = 16;
            upper_case = true;
            break;
        default:
            VERIFY_NOT_REACHED();
    }

    /* put the double value into the string-builder */
    TRY(try_put_f80(value,
                    base,
                    upper_case,
                    alignment(),
                    width().value_or(NumericLimits<usize>::min()),
                    precision().value_or(6),
                    alignment_fill(),
                    show_integer_sign()));
    return {};
}

Formatter<long double>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}
#endif

auto Formatter<char const*>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<char const*> {
    return Formatter<char const*>{ Cxx::move(format_applier) };
}

auto Formatter<char const*>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<char const*> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<char const*>::format(char const* value) const -> ErrorOr<void> {
    if ( display_as() == FormatParser::DisplayAs::Pointer ) {
        auto usize_formatter = Formatter<usize>::construct_from_format_applier(clone_format_applier());
        TRY(usize_formatter.format(bit_cast<usize>(value)));
    } else {
        auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
        TRY(sv_formatter.format(StringView{ value, __builtin_strlen(value) }));
    }

    return {};
}

Formatter<char const*>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<char*>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<char*> {
    return Formatter<char*>{ Cxx::move(format_applier) };
}

auto Formatter<char*>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<char*> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<char*>::format(char* value) const -> ErrorOr<void> {
    if ( display_as() == FormatParser::DisplayAs::Pointer ) {
        auto usize_formatter = Formatter<usize>::construct_from_format_applier(clone_format_applier());
        TRY(usize_formatter.format(bit_cast<usize>(value)));
    } else {
        auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
        TRY(sv_formatter.format(StringView{ const_cast<char const*>(value), __builtin_strlen(value) }));
    }

    return {};
}

Formatter<char*>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<SourceLocation>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<SourceLocation> {
    return Formatter<SourceLocation>{ Cxx::move(format_applier) };
}

auto Formatter<SourceLocation>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<SourceLocation> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<SourceLocation>::format(SourceLocation const& source_location) const -> ErrorOr<void> {
    auto file_path_sv = StringView{ source_location.file_path(), __builtin_strlen(source_location.file_path()) };
    if ( auto index_or_none = file_path_sv.find("MeetixOS"sv); index_or_none.is_present() ) {
        auto const index = index_or_none.unwrap();

        file_path_sv = file_path_sv.sub_string_view(index + "MeetixOS"sv.len() + 1);
    }

    auto const function_sv = StringView{ source_location.function(), __builtin_strlen(source_location.function()) };

    auto string_builder = StringBuilder::construct_empty();
    TRY(Text::format(string_builder, "{} {}() {}"sv, file_path_sv, function_sv, source_location.line()));

    auto formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
    TRY(formatter.format(string_builder.as_string_view()));
    return {};
}

Formatter<SourceLocation>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<ErrnoCode>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<ErrnoCode> {
    return Formatter<ErrnoCode>{ Cxx::move(format_applier) };
}

auto Formatter<ErrnoCode>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<ErrnoCode> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<ErrnoCode>::format(ErrnoCode const& errno_code) const -> ErrorOr<void> {
    static StringView const S_ERRNO_AS_STRING_VIEW[] = {
        "ENOERR"sv,          "E2BIG"sv,        "EACCES"sv,      "EADDRINUSE"sv,      "EADDRNOTAVAIL"sv, "EAFNOSUPPORT"sv, "EAGAIN"sv,       "EALREADY"sv,
        "EBADF"sv,           "EBADMSG"sv,      "EBUSY"sv,       "ECANCELED"sv,       "ECHILD"sv,        "ECONNABORTED"sv, "ECONNREFUSED"sv, "ECONNRESET"sv,
        "EDEADLK"sv,         "EDESTADDRREQ"sv, "EDOM"sv,        "EDQUOT"sv,          "EEXIST"sv,        "EFAULT"sv,       "EFBIG"sv,        "EHOSTUNREACH"sv,
        "EIDRM"sv,           "EILSEQ"sv,       "EINPROGRESS"sv, "EINTR"sv,           "EINVAL"sv,        "EIO"sv,          "EISCONN"sv,      "EISDIR"sv,
        "ELOOP"sv,           "EMFILE"sv,       "EMLINK"sv,      "EMSGSIZE"sv,        "EMULTIHOP"sv,     "ENAMETOOLONG"sv, "ENETDOWN"sv,     "ENETRESET"sv,
        "ENETUNREACH"sv,     "ENFILE"sv,       "ENOBUFS"sv,     "ENODEV"sv,          "ENOENT"sv,        "ENOEXEC"sv,      "ENOLCK"sv,       "ENOLINK"sv,
        "ENOMEM"sv,          "ENOMSG"sv,       "ENOPROTOOPT"sv, "ENOSPC"sv,          "ENOSYS"sv,        "ENOTCONN"sv,     "ENOTDIR"sv,      "ENOTEMPTY"sv,
        "ENOTRECOVERABLE"sv, "ENOTSOCK"sv,     "ENOTSUP"sv,     "ENOTTY"sv,          "ENXIO"sv,         "EOPNOTSUPP"sv,   "EOVERFLOW"sv,    "EOWNERDEAD"sv,
        "EPERM"sv,           "EPIPE"sv,        "EPROTO"sv,      "EPROTONOSUPPORT"sv, "EPROTOTYPE"sv,    "ERANGE"sv,       "EROFS"sv,        "ESPIPE"sv,
        "ESRCH"sv,           "ESTALE"sv,       "ETIMEDOUT"sv,   "ETXTBSY"sv,         "EWOULDBLOCK"sv,   "EXDEV"sv,        "ECUSTOM"sv
    };

    auto const errno_as_i32 = static_cast<i32>(errno_code);
    auto const errno_as_sv  = S_ERRNO_AS_STRING_VIEW[errno_as_i32];

    auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
    TRY(sv_formatter.format(errno_as_sv));

    return {};
}

Formatter<ErrnoCode>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<Error>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<Error> {
    return Formatter<Error>{ Cxx::move(format_applier) };
}

auto Formatter<Error>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Error> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<Error>::format(Error const& error) const -> ErrorOr<void> {
    auto string_builder = StringBuilder::construct_empty();

    TRY(Text::format(string_builder, "{}\n"sv, error.source_location()));
    if ( error.is_from_syscall() == Error::FromSyscall::Yes )
        TRY(Text::format(string_builder, "> System "sv));
    else
        TRY(Text::format(string_builder, "> User "sv));

    if ( error.string_literal() != nullptr )
        TRY(Text::format(string_builder, "{} - {}"sv, error.errno_code(), error.string_literal()));
    else
        TRY(Text::format(string_builder, "{}"sv, error.errno_code()));

    auto formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
    TRY(formatter.format(string_builder.as_string_view()));
    return {};
}

Formatter<Error>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<String>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<String> {
    return Formatter<String>{ Cxx::move(format_applier) };
}

auto Formatter<String>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<String> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<String>::format(String const& string) const -> ErrorOr<void> {
    auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
    TRY(sv_formatter.format(string.as_string_view()));

    return {};
}

Formatter<String>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

auto Formatter<StringBuilder>::construct_from_format_applier(FormatApplier format_applier) -> Formatter<StringBuilder> {
    return Formatter<StringBuilder>{ Cxx::move(format_applier) };
}

auto Formatter<StringBuilder>::construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<StringBuilder> {
    return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
}

auto Formatter<StringBuilder>::format(StringBuilder const& string_builder) const -> ErrorOr<void> {
    auto sv_formatter = Formatter<StringView>::construct_from_format_applier(clone_format_applier());
    TRY(sv_formatter.format(string_builder.as_string_view()));

    return {};
}

Formatter<StringBuilder>::Formatter(FormatApplier base_formatter)
    : FormatApplier{ Cxx::move(base_formatter) } {
}

} /* namespace TC::Text */