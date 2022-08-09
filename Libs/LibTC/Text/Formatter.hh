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
    auto try_put_padding(char fill, usize amount) -> ErrorOr<void>;
    auto try_put_literal(StringView value) -> ErrorOr<void>;
    auto try_put_string(StringView              value,
                        usize                   min_width      = 0,
                        usize                   max_width      = 0xffffff,
                        FormatParser::Alignment alignment      = FormatParser::Alignment::Left,
                        char                    alignment_fill = ' ') -> ErrorOr<void>;
    auto try_put_u64(u64                           value,
                     u8                            base             = 10,
                     FormatParser::ShowBase        show_base_prefix = FormatParser::ShowBase::No,
                     bool                          upper_case       = false,
                     FormatParser::ZeroPad         zero_pad         = FormatParser::ZeroPad::No,
                     usize                         min_width        = 0,
                     FormatParser::Alignment       alignment        = FormatParser::Alignment::Right,
                     char                          alignment_fill   = ' ',
                     FormatParser::ShowIntegerSign integer_sign     = FormatParser::ShowIntegerSign::IfNegative,
                     bool                          is_negative      = false) -> ErrorOr<void>;
    auto try_put_i64(i64                           value,
                     u8                            base             = 10,
                     FormatParser::ShowBase        show_base_prefix = FormatParser::ShowBase::No,
                     bool                          upper_case       = false,
                     FormatParser::ZeroPad         zero_pad         = FormatParser::ZeroPad::No,
                     usize                         min_width        = 0,
                     FormatParser::Alignment       alignment        = FormatParser::Alignment::Right,
                     char                          alignment_fill   = ' ',
                     FormatParser::ShowIntegerSign integer_sign     = FormatParser::ShowIntegerSign::IfNegative) -> ErrorOr<void>;
#ifndef IN_KERNEL
    auto try_put_f64(double                        value,
                     u8                            base           = 10,
                     bool                          upper_case     = false,
                     FormatParser::ZeroPad         zero_pad       = FormatParser::ZeroPad::No,
                     FormatParser::Alignment       alignment      = FormatParser::Alignment::Right,
                     usize                         min_width      = 0,
                     usize                         precision      = 6,
                     char                          alignment_fill = ' ',
                     FormatParser::ShowIntegerSign integer_sign   = FormatParser::ShowIntegerSign::IfNegative) -> ErrorOr<void>;
    auto try_put_f80(long double                   value,
                     u8                            base           = 10,
                     bool                          upper_case     = false,
                     FormatParser::Alignment       alignment      = FormatParser::Alignment::Right,
                     usize                         min_width      = 0,
                     usize                         precision      = 6,
                     char                          alignment_fill = ' ',
                     FormatParser::ShowIntegerSign integer_sign   = FormatParser::ShowIntegerSign::IfNegative) -> ErrorOr<void>;
#endif

    /**
     * @brief Getters
     */
    [[nodiscard]] auto string_builder() -> StringBuilder&;
    [[nodiscard]] auto alignment_fill() const -> char;
    [[nodiscard]] auto alignment() const -> FormatParser::Alignment;
    [[nodiscard]] auto show_integer_sign() const -> FormatParser::ShowIntegerSign;
    [[nodiscard]] auto show_base() const -> FormatParser::ShowBase;
    [[nodiscard]] auto zero_pad() const -> FormatParser::ZeroPad;
    [[nodiscard]] auto width() const -> Option<usize>;
    [[nodiscard]] auto precision() const -> Option<usize>;
    [[nodiscard]] auto display_as() const -> FormatParser::DisplayAs;
    [[nodiscard]] auto display_as_is_numeric() const -> bool;

    /**
     * @brief Setters
     */
    auto set_alignment_fill(char alignment_fill) -> void;
    auto set_alignment(FormatParser::Alignment alignment) -> void;
    auto set_show_integer_sign(FormatParser::ShowIntegerSign show_integer_sign) -> void;
    auto set_show_base(FormatParser::ShowBase show_base) -> void;
    auto set_zero_pad(FormatParser::ZeroPad zero_pad) -> void;
    auto set_width(Option<usize> width) -> void;
    auto set_precision(Option<usize> precision) -> void;
    auto set_display_as(FormatParser::DisplayAs display_as) -> void;

private:
    static auto convert_unsigned_to_chars(u64 value, char to_chars_buffer[128], u8 base, bool upper_case) -> usize;

private:
    StringBuilder&               m_string_builder;
    FormatParser::Specifications m_specifications{};
};

template<typename T, typename>
class Formatter {
public:
    using NoFormatterAvailable = void;
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
    [[nodiscard]] auto format(nullptr_t) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(StringView value) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(T value) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(bool value) const -> ErrorOr<void>;
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
    [[nodiscard]] auto format(char value) const -> ErrorOr<void>;
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
    [[nodiscard]] auto format(float value) const -> ErrorOr<void>;
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
    [[nodiscard]] auto format(double value) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(long double value) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(T* value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(char const* value) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(unsigned char const* value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(List<T> const& value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(Set<T> const& value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(OrderedSet<T> const& value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(Map<K, T> const& value) -> ErrorOr<void> {
        TRY(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : value ) {
            Formatter<K> key_formatter{ *this };
            Formatter<T> value_formatter{ *this };

            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(key_formatter.format(pair.m_key));
            TRY(try_put_literal(": "sv));
            TRY(value_formatter.format(pair.m_value));
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
    [[nodiscard]] auto format(OrderedMap<K, T> const& value) -> ErrorOr<void> {
        TRY(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : value ) {
            Formatter<K> key_formatter{ *this };
            Formatter<T> value_formatter{ *this };

            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(key_formatter.format(pair.m_key));
            TRY(try_put_literal(": "sv));
            TRY(value_formatter.format(pair.m_value));
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
    [[nodiscard]] auto format(Range<T> const& value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(RangeInclusive<T> const& value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(StringBuilder const& value) -> ErrorOr<void>;
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
    [[nodiscard]] auto format(Vector<T> const& value) -> ErrorOr<void> {
        if ( display_as() == FormatParser::DisplayAs::Pointer ) {
            Formatter<T*> formatter{ *this };
            return formatter.format(value.data());
        }

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
    [[nodiscard]] auto format(Result<T, E> const& value) -> ErrorOr<void> {
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
    [[nodiscard]] auto format(Option<T> const& value) -> ErrorOr<void> {
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