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

#include <LibTC/Forward.hh>

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

class FormatApplier {
    TC_DENY_COPY(FormatApplier);

public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_string_builder(StringBuilder& string_builder) -> FormatApplier;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> FormatApplier;

    /**
     * @brief Move constructors
     */
    FormatApplier(FormatApplier&& rhs) noexcept;
    auto operator=(FormatApplier&& rhs) noexcept -> FormatApplier&;

    /**
     * @brief Swap content of this FormatApplier
     */
    auto swap(FormatApplier& rhs) noexcept -> void;

protected:
    /**
     * @brief Cloning
     */
    [[nodiscard]] auto clone_format_applier() const -> FormatApplier;

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
    explicit FormatApplier(StringBuilder& string_builder, FormatParser::Result result);

private:
    static auto convert_unsigned_to_chars(u64 value, char to_chars_buffer[128], u8 base, bool upper_case) -> usize;

private:
    StringBuilder&       m_string_builder;
    FormatParser::Result m_parser_result{};
};

template<typename T, typename>
class Formatter {
public:
    using NoFormatterAvailable = void;
};

template<>
class Formatter<nullptr_t> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<nullptr_t>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<nullptr_t>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(nullptr_t) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<StringView> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<StringView>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<StringView>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(StringView value) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<Integral T>
class Formatter<T> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<T>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<T>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(T value) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<bool> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<bool>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<bool>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(bool value) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<char> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<char>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<char>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(char value) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

#ifndef IN_KERNEL
template<>
class Formatter<float> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<float>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<float>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(float value) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<double> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<double>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<double>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(double value) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<long double> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<long double>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<long double>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(long double value) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};
#endif

template<typename T>
class Formatter<T*> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<T*> {
        return Formatter<T*>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<T*> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(T* value) -> ErrorOr<void> {
        /* show as pointer a pointer */
        if ( display_as() == FormatParser::DisplayAs::Default )
            set_display_as(FormatParser::DisplayAs::Pointer);

        /* forward to the integral formatter */
        auto usize_formatter = Formatter<usize>::construct_from_format_applier(clone_format_applier());
        TRY(usize_formatter.format(bit_cast<usize>(value)));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<>
class Formatter<char const*> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<char const*>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<char const*>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(char const* value) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<char*> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<char*>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<char*>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(char* value) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<Error> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Error>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Error>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Error const& value) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<typename T>
class Formatter<List<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<List<T>> {
        return Formatter<List<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<List<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(List<T> const& list) -> ErrorOr<void> {
        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : list ) {
            auto e_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(e_formatter.format(e));
        }
        TRY(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<Set<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Set<T>> {
        return Formatter<Set<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Set<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Set<T> const& set) -> ErrorOr<void> {
        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : set ) {
            auto e_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(e_formatter.format(e));
        }
        TRY(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<OrderedSet<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<OrderedSet<T>> {
        return Formatter<OrderedSet<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<OrderedSet<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(OrderedSet<T> const& ordered_set) -> ErrorOr<void> {
        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : ordered_set ) {
            auto e_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(e_formatter.format(e));
        }
        TRY(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename K, typename T>
class Formatter<Map<K, T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Map<K, T>> {
        return Formatter<Map<K, T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Map<K, T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Map<K, T> const& ordered_map) -> ErrorOr<void> {
        TRY(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : ordered_map ) {
            auto k_formatter = Formatter<K>::construct_from_format_applier(clone_format_applier());
            auto v_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());

            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(k_formatter.format(pair.m_key));
            TRY(try_put_literal(": "sv));
            TRY(v_formatter.format(pair.m_value));
        }
        TRY(try_put_literal(" }"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename K, typename T>
class Formatter<OrderedMap<K, T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<OrderedMap<K, T>> {
        return Formatter<OrderedMap<K, T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<OrderedMap<K, T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(OrderedMap<K, T> const& ordered_map) -> ErrorOr<void> {
        TRY(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : ordered_map ) {
            auto k_formatter = Formatter<K>::construct_from_format_applier(clone_format_applier());
            auto v_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());

            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(k_formatter.format(pair.m_key));
            TRY(try_put_literal(": "sv));
            TRY(v_formatter.format(pair.m_value));
        }
        TRY(try_put_literal(" }"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<Range<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Range<T>> {
        return Formatter<Range<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Range<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Range<T> const& range) -> ErrorOr<void> {
        auto formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());

        TRY(formatter.format(range.begin().value()));
        TRY(try_put_literal(".."sv));
        TRY(formatter.format(range.end().value()));

        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<RangeInclusive<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<RangeInclusive<T>> {
        return Formatter<RangeInclusive<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<RangeInclusive<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(RangeInclusive<T> const& range_inclusive) -> ErrorOr<void> {
        auto formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());

        TRY(formatter.format(range_inclusive.begin().value()));
        TRY(try_put_literal("..="sv));
        TRY(formatter.format(range_inclusive.end().value() - 1));

        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<>
class Formatter<String>  : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<String>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<String>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(String const& string) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<>
class Formatter<StringBuilder> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<StringBuilder>;
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<StringBuilder>;

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(StringBuilder const& string_builder) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier format_applier);
};

template<typename T>
class Formatter<Vector<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Vector<T>> {
        return Formatter<Vector<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Vector<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Vector<T> const& vector) -> ErrorOr<void> {
        if ( display_as() == FormatParser::DisplayAs::Pointer ) {
            Formatter<T const*> formatter{ *this };
            TRY(formatter.format(vector.raw_data()));

            return {};
        }

        TRY(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : vector ) {
            auto e_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());
            if ( !is_first )
                TRY(try_put_literal(", "sv));
            else
                is_first = false;

            TRY(e_formatter.format(e));
        }
        TRY(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T, typename E>
class Formatter<Result<T, E>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Result<T, E>> {
        return Formatter<Result<T, E>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Result<T, E>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Result<T, E> const& result) -> ErrorOr<void> {
        if ( result.is_value() ) {
            auto v_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());

            TRY(try_put_literal("Value("sv));
            TRY(v_formatter.format(result.value()));
            TRY(try_put_literal(")"sv));
        } else {
            auto e_formatter = Formatter<E>::construct_from_format_applier(clone_format_applier());

            TRY(try_put_literal("Error("sv));
            TRY(e_formatter.format(result.error()));
            TRY(try_put_literal(")"sv));
        }

        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<Option<T>> : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]] static auto construct_from_format_applier(FormatApplier format_applier) -> Formatter<Option<T>> {
        return Formatter<Option<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]] static auto construct_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Option<T>> {
        return construct_from_format_applier(FormatApplier::construct_from_parser_result(string_builder, Cxx::move(result)));
    }

    /**
     * @brief Performs the format on the given string-builder
     */
    [[nodiscard]] auto format(Option<T> const& option) -> ErrorOr<void> {
        if ( option.is_present() ) {
            auto v_formatter = Formatter<T>::construct_from_format_applier(clone_format_applier());

            TRY(try_put_literal("Some("sv));
            TRY(v_formatter.format(option.value()));
            TRY(try_put_literal(")"sv));
        } else
            TRY(try_put_literal("None"sv));

        return {};
    }
};

} /* namespace Text */

using Text::Formatter;

} /* namespace TC */