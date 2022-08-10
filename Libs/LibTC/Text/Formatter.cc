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
#include <LibTC/Text/Format.hh>
#include <LibTC/Text/Formatter.hh>

#ifndef IN_KERNEL
#    include <LibMath/math.h>
#endif

namespace TC::Text {

BaseFormatter::BaseFormatter(StringBuilder& string_builder)
    : m_string_builder{ string_builder } {
}

BaseFormatter::BaseFormatter(StringBuilder& string_builder, FormatParser::Specifications specifications)
    : m_string_builder{ string_builder }
    , m_specifications{ Cxx::move(specifications) } {
}

auto BaseFormatter::try_put_padding(char fill, usize amount) -> ErrorOr<void> {
    for ( [[maybe_unused]] usize i : Range{ 0u, amount } )
        TRY(m_string_builder.try_append(fill));
    return {};
}

auto BaseFormatter::try_put_literal(StringView value) -> ErrorOr<void> {
    for ( usize i = 0; i < value.len(); ++i ) {
        TRY(m_string_builder.try_append(value[i]));

        /* skip escaped placeholders */
        auto const sub_string_view = value.sub_string_view(i);
        if ( sub_string_view.starts_with("{{"sv) || sub_string_view.starts_with("}}"sv) )
            ++i;
    }
    return {};
}

auto BaseFormatter::try_put_string(StringView              value,
                                   usize                   min_width,
                                   usize                   max_width,
                                   FormatParser::Alignment alignment,
                                   char                    alignment_fill) -> ErrorOr<void> {
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

auto BaseFormatter::try_put_u64(u64                           value,
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

auto BaseFormatter::try_put_i64(i64                           value,
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
auto BaseFormatter::try_put_f64(double                        value,
                                u8                            base,
                                bool                          upper_case,
                                FormatParser::ZeroPad         zero_pad,
                                FormatParser::Alignment       alignment,
                                usize                         min_width,
                                usize                         precision,
                                char                          alignment_fill,
                                FormatParser::ShowIntegerSign integer_sign) -> ErrorOr<void> {
    auto          string_builder = StringBuilder::construct_empty();
    BaseFormatter base_formatter{ string_builder };

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
    TRY(base_formatter.try_put_u64(static_cast<u64>(value),
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
            TRY(base_formatter.try_put_u64(static_cast<u64>(value),
                                           base,
                                           FormatParser::ShowBase::No,
                                           upper_case,
                                           FormatParser::ZeroPad::Yes,
                                           visible_precision));
        }
        if ( zero_pad == FormatParser::ZeroPad::Yes && (precision - visible_precision) > 0 ) {
            TRY(base_formatter
                    .try_put_u64(0, base, FormatParser::ShowBase::No, false, FormatParser::ZeroPad::Yes, precision - visible_precision));
        }
    }

    /* put into this builder the values */
    TRY(try_put_string(string_builder.as_string_view(), min_width, 0xffffff, alignment, alignment_fill));
    return {};
}

auto BaseFormatter::try_put_f80(long double                   value,
                                u8                            base,
                                bool                          upper_case,
                                FormatParser::Alignment       alignment,
                                usize                         min_width,
                                usize                         precision,
                                char                          alignment_fill,
                                FormatParser::ShowIntegerSign integer_sign) -> ErrorOr<void> {
    auto          string_builder = StringBuilder::construct_empty();
    BaseFormatter base_formatter{ string_builder };

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
    TRY(base_formatter.try_put_u64(static_cast<u64>(value),
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
            TRY(base_formatter.try_put_u64(static_cast<u64>(value),
                                           base,
                                           FormatParser::ShowBase::No,
                                           upper_case,
                                           FormatParser::ZeroPad::Yes,
                                           visible_precision));
        }
    }

    /* put into this builder the values */
    TRY(try_put_string(string_builder.as_string_view(), min_width, 0xffffff, alignment, alignment_fill));
    return {};
}
#endif

auto BaseFormatter::string_builder() -> StringBuilder& {
    return m_string_builder;
}

auto BaseFormatter::alignment_fill() const -> char {
    return m_specifications.m_alignment_fill;
}

auto BaseFormatter::alignment() const -> FormatParser::Alignment {
    return m_specifications.m_alignment;
}

auto BaseFormatter::show_integer_sign() const -> FormatParser::ShowIntegerSign {
    return m_specifications.m_show_integer_sign;
}

auto BaseFormatter::show_base() const -> FormatParser::ShowBase {
    return m_specifications.m_show_base;
}

auto BaseFormatter::zero_pad() const -> FormatParser::ZeroPad {
    return m_specifications.m_zero_pad;
}

auto BaseFormatter::width() const -> Option<usize> {
    return m_specifications.m_width;
}

auto BaseFormatter::precision() const -> Option<usize> {
    return m_specifications.m_precision;
}

auto BaseFormatter::display_as() const -> FormatParser::DisplayAs {
    return m_specifications.m_display_as;
}

auto BaseFormatter::display_as_is_numeric() const -> bool {
    return m_specifications.display_as_is_numeric();
}

auto BaseFormatter::set_alignment_fill(char alignment_fill) -> void {
    m_specifications.m_alignment_fill = alignment_fill;
}

auto BaseFormatter::set_alignment(FormatParser::Alignment alignment) -> void {
    m_specifications.m_alignment = alignment;
}

auto BaseFormatter::set_show_integer_sign(FormatParser::ShowIntegerSign show_integer_sign) -> void {
    m_specifications.m_show_integer_sign = show_integer_sign;
}

auto BaseFormatter::set_show_base(FormatParser::ShowBase show_base) -> void {
    m_specifications.m_show_base = show_base;
}

auto BaseFormatter::set_zero_pad(FormatParser::ZeroPad zero_pad) -> void {
    m_specifications.m_zero_pad = zero_pad;
}

auto BaseFormatter::set_width(Option<usize> width) -> void {
    m_specifications.m_width = width;
}

auto BaseFormatter::set_precision(Option<usize> precision) -> void {
    m_specifications.m_precision = precision;
}

auto BaseFormatter::set_display_as(FormatParser::DisplayAs display_as) -> void {
    m_specifications.m_display_as = display_as;
}

auto BaseFormatter::convert_unsigned_to_chars(u64 value, char to_chars_buffer[128], u8 base, bool upper_case) -> usize {
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
        swap(to_chars_buffer[i], to_chars_buffer[used_chars - i - 1]);

    return used_chars;
}

Formatter<nullptr_t>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

auto Formatter<nullptr_t>::format(nullptr_t) -> ErrorOr<void> {
    TRY(try_put_string("nullptr"sv, width().value_or(0), precision().value_or(0xffffff), alignment(), alignment_fill()));
    return {};
}

Formatter<StringView>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

auto Formatter<StringView>::format(StringView value) -> ErrorOr<void> {
    if ( show_integer_sign() != FormatParser::ShowIntegerSign::IfNegative )
        VERIFY_NOT_REACHED();
    if ( show_base() != FormatParser::ShowBase::No )
        VERIFY_NOT_REACHED();
    if ( zero_pad() != FormatParser::ZeroPad::No )
        VERIFY_NOT_REACHED();
    if ( display_as() != FormatParser::DisplayAs::Default && display_as() != FormatParser::DisplayAs::String
         && display_as() != FormatParser::DisplayAs::Char )
        VERIFY_NOT_REACHED();

    TRY(try_put_string(value, width().value_or(0), precision().value_or(0xfffff), alignment(), alignment_fill()));
    return {};
}

template<Integral T>
Formatter<T>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

template<Integral T>
auto Formatter<T>::format(T value) -> ErrorOr<void> {
    if ( display_as() == FormatParser::DisplayAs::Char ) {
        VERIFY_GREATER_EQUAL(value, 0);
        VERIFY_LESS_EQUAL(value, 127);

        /* display as string */
        set_display_as(FormatParser::DisplayAs::String);

        /* forward to the string-view formatter */
        Formatter<StringView> formatter{ *this };
        TRY(formatter.format(StringView{ bit_cast<char const*>(&value), 1 }));

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
                        width().value_or(0),
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
                        width().value_or(0),
                        alignment(),
                        alignment_fill(),
                        show_integer_sign()));
    }

    return {};
}

template class Formatter<u8>;
template class Formatter<u16>;
template class Formatter<u32>;
template class Formatter<u64>;

template class Formatter<i8>;
template class Formatter<i16>;
template class Formatter<i32>;
template class Formatter<i64>;

Formatter<bool>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

auto Formatter<bool>::format(bool value) const -> ErrorOr<void> {
    if ( display_as_is_numeric() ) {
        Formatter<u8> formatter{ *this };
        TRY(formatter.format(static_cast<u8>(value)));
    } else {
        Formatter<StringView> formatter{ *this };
        TRY(formatter.format(value ? "true"sv : "false"sv));
    }

    return {};
}

Formatter<char>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

auto Formatter<char>::format(char value) const -> ErrorOr<void> {
    if ( display_as_is_numeric() ) {
        Formatter<i8> formatter{ *this };
        TRY(formatter.format(value));
    } else {
        Formatter<StringView> formatter{ *this };
        TRY(formatter.format(StringView{ &value, 1 }));
    }

    return {};
}

#ifndef IN_KERNEL
Formatter<float>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

auto Formatter<float>::format(float value) const -> ErrorOr<void> {
    Formatter<double> formatter{ *this };
    TRY(formatter.format(static_cast<double>(value)));

    return {};
}

Formatter<double>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
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
                    width().value_or(0),
                    precision().value_or(6),
                    alignment_fill(),
                    show_integer_sign()));
    return {};
}

Formatter<long double>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
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
                    width().value_or(0),
                    precision().value_or(6),
                    alignment_fill(),
                    show_integer_sign()));
    return {};
}
#endif

auto Formatter<char const*>::format(char const* value) -> ErrorOr<void> {
    if ( display_as() == FormatParser::DisplayAs::Pointer ) {
        Formatter<usize> formatter{ *this };
        TRY(formatter.format(bit_cast<usize>(value)));
    } else
        TRY(Formatter<StringView>::format({ value, __builtin_strlen(value) }));

    return {};
}

auto Formatter<StringBuilder>::format(StringBuilder const& value) -> ErrorOr<void> {
    return Formatter<StringView>::format(value.as_string_view());
}

Formatter<Error>::Formatter(BaseFormatter base_formatter)
    : BaseFormatter{ Cxx::move(base_formatter) } {
}

auto Formatter<Error>::format(Error const& value) const -> ErrorOr<void> {
    auto string_builder = StringBuilder::construct_empty();
    TRY(Text::format(string_builder, "{}: {}"sv, value.string_literal(), static_cast<i32>(value.os_error())));

    Formatter<StringView> formatter{ *this };
    TRY(formatter.format(string_builder.as_string_view()));
    return {};
}

} /* namespace TC::Text */