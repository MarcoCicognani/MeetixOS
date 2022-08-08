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

#pragma once

#include <LibTC/Assertions.hh>
#include <LibTC/BitCast.hh>
#include <LibTC/Collection/List.hh>
#include <LibTC/Collection/Map.hh>
#include <LibTC/Collection/Range.hh>
#include <LibTC/Collection/Set.hh>
#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringBuilder.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Collection/Vector.hh>
#include <LibTC/Concept.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Functional/Result.hh>
#include <LibTC/Meta.hh>
#include <LibTC/Text/FormatParser.hh>

namespace TC {
namespace Text {

class BaseFormatter {
public:
    /**
     * @brief Constructor
     */
    explicit BaseFormatter(StringBuilder& string_builder);
    explicit BaseFormatter(StringBuilder& string_builder, FormatParser::Specifications specifications);

protected:
    /**
     * @brief put formatting functions
     */
    ErrorOr<void> try_put_padding(char fill, usize amount);
    ErrorOr<void> try_put_literal(StringView value);
    ErrorOr<void> try_put_string(StringView              value,
                                 usize                   min_width      = 0,
                                 usize                   max_width      = 0xffffff,
                                 FormatParser::Alignment alignment      = FormatParser::Alignment::Left,
                                 char                    alignment_fill = ' ');
    ErrorOr<void> try_put_u64(u64                           value,
                              u8                            base             = 10,
                              FormatParser::ShowBase        show_base_prefix = FormatParser::ShowBase::No,
                              bool                          upper_case       = false,
                              FormatParser::ZeroPad         zero_pad         = FormatParser::ZeroPad::No,
                              usize                         min_width        = 0,
                              FormatParser::Alignment       alignment        = FormatParser::Alignment::Right,
                              char                          alignment_fill   = ' ',
                              FormatParser::ShowIntegerSign integer_sign     = FormatParser::ShowIntegerSign::IfNegative,
                              bool                          is_negative      = false);
    ErrorOr<void> try_put_i64(i64                           value,
                              u8                            base             = 10,
                              FormatParser::ShowBase        show_base_prefix = FormatParser::ShowBase::No,
                              bool                          upper_case       = false,
                              FormatParser::ZeroPad         zero_pad         = FormatParser::ZeroPad::No,
                              usize                         min_width        = 0,
                              FormatParser::Alignment       alignment        = FormatParser::Alignment::Right,
                              char                          alignment_fill   = ' ',
                              FormatParser::ShowIntegerSign integer_sign     = FormatParser::ShowIntegerSign::IfNegative);
#ifndef IN_KERNEL
    ErrorOr<void> try_put_f64(double                        value,
                              u8                            base           = 10,
                              bool                          upper_case     = false,
                              FormatParser::ZeroPad         zero_pad       = FormatParser::ZeroPad::No,
                              FormatParser::Alignment       alignment      = FormatParser::Alignment::Right,
                              usize                         min_width      = 0,
                              usize                         precision      = 6,
                              char                          alignment_fill = ' ',
                              FormatParser::ShowIntegerSign integer_sign   = FormatParser::ShowIntegerSign::IfNegative);
    ErrorOr<void> try_put_f80(long double                   value,
                              u8                            base           = 10,
                              bool                          upper_case     = false,
                              FormatParser::Alignment       alignment      = FormatParser::Alignment::Right,
                              usize                         min_width      = 0,
                              usize                         precision      = 6,
                              char                          alignment_fill = ' ',
                              FormatParser::ShowIntegerSign integer_sign   = FormatParser::ShowIntegerSign::IfNegative);
#endif

    /**
     * @brief Getters
     */
    [[nodiscard]] StringBuilder&                string_builder();
    [[nodiscard]] char                          alignment_fill() const;
    [[nodiscard]] FormatParser::Alignment       alignment() const;
    [[nodiscard]] FormatParser::ShowIntegerSign show_integer_sign() const;
    [[nodiscard]] FormatParser::ShowBase        show_base() const;
    [[nodiscard]] FormatParser::ZeroPad         zero_pad() const;
    [[nodiscard]] Option<usize>                 width() const;
    [[nodiscard]] Option<usize>                 precision() const;
    [[nodiscard]] FormatParser::DisplayAs       display_as() const;
    [[nodiscard]] bool                          display_as_is_numeric() const;

    /**
     * @brief Setters
     */
    void set_alignment_fill(char alignment_fill);
    void set_alignment(FormatParser::Alignment alignment);
    void set_show_integer_sign(FormatParser::ShowIntegerSign show_integer_sign);
    void set_show_base(FormatParser::ShowBase show_base);
    void set_zero_pad(FormatParser::ZeroPad zero_pad);
    void set_width(Option<usize> width);
    void set_precision(Option<usize> precision);
    void set_display_as(FormatParser::DisplayAs display_as);

private:
    static usize convert_unsigned_to_chars(u64 value, char to_chars_buffer[128], u8 base, bool upper_case);

private:
    StringBuilder&               m_string_builder;
    FormatParser::Specifications m_specifications{};
};

template<typename T, typename = void>
class Formatter {
public:
    using no_formatter_available = void;
};

template<>
class Formatter<nullptr_t> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(nullptr_t);
};

template<>
class Formatter<StringView> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(StringView value);
};

template<Integral T>
class Formatter<T> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(T value);
};

template<>
class Formatter<bool> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(bool value) const;
};

template<>
class Formatter<char> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(char value) const;
};

#ifndef IN_KERNEL
template<>
class Formatter<float> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(float value) const;
};

template<>
class Formatter<double> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(double value);
};

template<>
class Formatter<long double> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter);

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(long double value);
};
#endif

template<typename T>
class Formatter<T*> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(T* value) {
        /* show as pointer a pointer */
        if ( display_as() == FormatParser::DisplayAs::Default )
            set_display_as(FormatParser::DisplayAs::Pointer);

        /* forward to the integral formatter */
        Formatter<usize> formatter{ *this };
        return formatter.format(bit_cast<usize>(value));
    }
};

template<>
class Formatter<char const*> : public Formatter<StringView> {
public:
    using Formatter<StringView>::Formatter;

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(char const* value);
};

template<>
class Formatter<char*> : public Formatter<char const*> {
public:
    using Formatter<char const*>::Formatter;
};

template<usize SIZE>
class Formatter<char[SIZE]> : public Formatter<char const*> {
public:
    using Formatter<char const*>::Formatter;
};

template<usize SIZE>
class Formatter<unsigned char[SIZE]> : public Formatter<StringView> {
public:
    using Formatter<StringView>::Formatter;

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(unsigned char const* value) {
        if ( display_as() == FormatParser::DisplayAs::Pointer ) {
            Formatter<usize> formatter{ *this };
            return formatter.format(bit_cast<usize>(value));
        } else
            return Formatter<StringView>::format(StringView{ bit_cast<char const*>(value), SIZE });
    }
};

template<typename T>
class Formatter<List<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(List<T> const& value) {
        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& element : value.iter() ) {
            Formatter<T> element_formatter{ *this };
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(element_formatter.format(element));
        }
        return try_put_literal(" ]"sv);
    }
};

template<typename T>
class Formatter<Set<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(Set<T> const& value) {
        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& element : value ) {
            Formatter<T> element_formatter{ *this };
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(element_formatter.format(element));
        }
        return try_put_literal(" ]"sv);
    }
};

template<typename T>
class Formatter<OrderedSet<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(OrderedSet<T> const& value) {
        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& element : value ) {
            Formatter<T> element_formatter{ *this };
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(element_formatter.format(element));
        }
        return try_put_literal(" ]"sv);
    }
};

template<typename K, typename T>
class Formatter<Map<K, T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(Map<K, T> const& value) {
        TRY(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : value ) {
            Formatter<K> key_formatter{ *this };
            Formatter<T> value_formatter{ *this };

            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(key_formatter.format(value.key()));
            TRY(try_put_literal(": "sv));
            TRY(value_formatter.format(value.value()));
        }
        return try_put_literal(" }"sv);
    }
};

template<typename K, typename T>
class Formatter<OrderedMap<K, T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(OrderedMap<K, T> const& value) {
        TRY(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : value ) {
            Formatter<K> key_formatter{ *this };
            Formatter<T> value_formatter{ *this };

            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(key_formatter.format(value.key()));
            TRY(try_put_literal(": "sv));
            TRY(value_formatter.format(value.value()));
        }
        return try_put_literal(" }"sv);
    }
};

template<typename T>
class Formatter<Range<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(Range<T> const& value) {
        Formatter<T> formatter{ *this };
        TRY(formatter.format(value.begin().value()));
        TRY(try_put_literal(".."sv));
        return formatter.format(value.end().value());
    }
};

template<typename T>
class Formatter<RangeInclusive<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(RangeInclusive<T> const& value) {
        Formatter<T> formatter{ *this };
        TRY(formatter.format(value.begin().value()));
        TRY(try_put_literal("..="sv));
        return formatter.format(value.end().value() - 1);
    }
};

template<>
class Formatter<String> : public Formatter<StringView> {
public:
    using Formatter<StringView>::Formatter;
};

template<>
class Formatter<StringBuilder> : public Formatter<StringView> {
public:
    using Formatter<StringView>::Formatter;

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(StringBuilder const& value);
};

template<typename T>
class Formatter<Vector<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(Vector<T> const& value) {
        if ( display_as() == FormatParser::DisplayAs::Pointer ) {
            Formatter<T*> formatter{ *this };
            return formatter.format(value.data());
        }

        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& element : value.iter() ) {
            Formatter<T> element_formatter{ *this };
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(element_formatter.format(element));
        }
        return try_put_literal(" ]"sv);
    }
};

template<typename T, typename E>
class Formatter<Result<T, E>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(Result<T, E> const& value) {
        if ( value.is_value() ) {
            Formatter<T> value_formatter{ *this };

            TRY(try_put_literal("Value("sv));
            TRY(value_formatter.format(value.value()));
            return try_put_literal(")"sv);
        } else {
            Formatter<E> error_formatter{ *this };

            TRY(try_put_literal("Error("sv));
            TRY(error_formatter.format(value.error()));
            return try_put_literal(")"sv);
        }
    }
};

template<typename T>
class Formatter<Option<T>> : public BaseFormatter {
public:
    /**
     * @brief Constructors
     */
    using BaseFormatter::BaseFormatter;
    explicit Formatter(BaseFormatter base_formatter)
        : BaseFormatter{ Cxx::move(base_formatter) } {
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    ErrorOr<void> format(Option<T> const& value) {
        if ( value.is_present() ) {
            Formatter<T> value_formatter{ *this };

            TRY(try_put_literal("Some("sv));
            TRY(value_formatter.format(value.value()));
            return try_put_literal(")"sv);
        } else
            return try_put_literal("None"sv);
    }
};

} /* namespace Text */

using Text::Formatter;

} /* namespace TC */