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

#include <CCLang/Forward.hh>

#include <CCLang/Alloc/List.hh>
#include <CCLang/Alloc/Map.hh>
#include <CCLang/Alloc/Set.hh>
#include <CCLang/Alloc/String.hh>
#include <CCLang/Alloc/StringBuilder.hh>
#include <CCLang/Alloc/Text/FormatParser.hh>
#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Core/Meta.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Result.hh>
#include <CCLang/Lang/StringView.hh>

class FormatApplier {
    TCDenyCopy$(FormatApplier);

public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_string_builder(StringBuilder&) -> FormatApplier;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> FormatApplier;

    virtual ~FormatApplier() = default;

    /**
     * @brief Cxx::Move constructors
     */
    FormatApplier(FormatApplier&&);
    auto operator=(FormatApplier&&) -> FormatApplier&;

    /**
     * @brief Swap content of this FormatApplier
     */
    auto swap(FormatApplier&) -> void;

protected:
    /**
     * @brief Cloning
     */
    [[nodiscard]]
    auto clone_format_applier() const -> FormatApplier;

    /**
     * @brief put formatting functions
     */
    auto try_put_padding(char fill, usize amount) -> ErrorOr<void>;
    auto try_put_literal(StringView literals_view) -> ErrorOr<void>;
    auto try_put_string(StringView value,
                        usize      min_width    = 0,
                        usize      max_width    = 0xffffff,
                        FormatParser::Alignment = FormatParser::Alignment::Left,
                        char align_fill         = ' ') -> ErrorOr<void>;
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
    [[nodiscard]]
    auto string_builder() -> StringBuilder&;
    [[nodiscard]]
    auto alignment_fill() const -> char;
    [[nodiscard]]
    auto alignment() const -> FormatParser::Alignment;
    [[nodiscard]]
    auto show_integer_sign() const -> FormatParser::ShowIntegerSign;
    [[nodiscard]]
    auto show_base() const -> FormatParser::ShowBase;
    [[nodiscard]]
    auto zero_pad() const -> FormatParser::ZeroPad;
    [[nodiscard]]
    auto width() const -> Option<usize>;
    [[nodiscard]]
    auto precision() const -> Option<usize>;
    [[nodiscard]]
    auto display_as() const -> FormatParser::DisplayAs;
    [[nodiscard]]
    auto display_as_is_numeric() const -> bool;

    /**
     * @brief Setters
     */
    auto set_alignment_fill(char) -> void;
    auto set_alignment(FormatParser::Alignment) -> void;
    auto set_show_integer_sign(FormatParser::ShowIntegerSign) -> void;
    auto set_show_base(FormatParser::ShowBase) -> void;
    auto set_zero_pad(FormatParser::ZeroPad) -> void;
    auto set_width(Option<usize>) -> void;
    auto set_precision(Option<usize>) -> void;
    auto set_display_as(FormatParser::DisplayAs) -> void;

private:
    explicit FormatApplier(StringBuilder&, FormatParser::Result);

private:
    static auto convert_unsigned_to_chars(u64 value, char to_chars_buffer[128], u8 base, bool upper_case) -> usize;

private:
    StringBuilder&       m_string_builder;
    FormatParser::Result m_parser_result{};
};

template<typename T, typename>
class Formatter final {
public:
    using NoFormatterAvailable = void;
};

template<>
class Formatter<nullptr_t> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<nullptr_t>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<nullptr_t>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(nullptr_t) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<StringView> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<StringView>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<StringView>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(StringView) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<Integral T>
class Formatter<T> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<T>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<T>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(T) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<bool> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<bool>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<bool>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(bool) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<char> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<char>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<char>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(char) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

#ifndef IN_KERNEL
template<>
class Formatter<f32> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<f32>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<f32>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(f32) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<f64> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<f64>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<f64>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(f64) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<f80> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<f80>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<f80>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(f80) -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};
#endif

template<typename T>
class Formatter<T*> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<T*> {
        return Formatter<T*>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<T*> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(T* value) -> ErrorOr<void> {
        /* show as pointer a pointer */
        if ( display_as() == FormatParser::DisplayAs::Default )
            set_display_as(FormatParser::DisplayAs::Pointer);

        /* forward to the integral formatter */
        auto usize_formatter = Formatter<usize>::new_from_format_applier(clone_format_applier());
        try$(usize_formatter.format(Cxx::bit_cast<usize>(value)));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<>
class Formatter<char const*> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<char const*>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<char const*>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(char const*) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<SourceLocation> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<SourceLocation>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<SourceLocation>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(SourceLocation const&) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<ErrorCode> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<ErrorCode>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<ErrorCode>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(ErrorCode const&) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<Error> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<Error>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<Error>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Error const&) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<typename T>
class Formatter<List<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<List<T>> {
        return Formatter<List<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<List<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(List<T> const& list) -> ErrorOr<void> {
        try$(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : list ) {
            auto e_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());
            if ( !is_first )
                try$(try_put_literal(", "sv));
            else
                is_first = false;

            try$(e_formatter.format(e));
        }
        try$(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<Set<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<Set<T>> {
        return Formatter<Set<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Set<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Set<T> const& set) -> ErrorOr<void> {
        try$(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : set ) {
            auto e_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());
            if ( !is_first )
                try$(try_put_literal(", "sv));
            else
                is_first = false;

            try$(e_formatter.format(e));
        }
        try$(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<OrderedSet<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<OrderedSet<T>> {
        return Formatter<OrderedSet<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<OrderedSet<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(OrderedSet<T> const& ordered_set) -> ErrorOr<void> {
        try$(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : ordered_set ) {
            auto e_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());
            if ( !is_first )
                try$(try_put_literal(", "sv));
            else
                is_first = false;

            try$(e_formatter.format(e));
        }
        try$(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename K, typename T>
class Formatter<Map<K, T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<Map<K, T>> {
        return Formatter<Map<K, T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Map<K, T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Map<K, T> const& ordered_map) -> ErrorOr<void> {
        try$(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : ordered_map ) {
            auto k_formatter = Formatter<K>::new_from_format_applier(clone_format_applier());
            auto v_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());

            if ( !is_first )
                try$(try_put_literal(", "sv));
            else
                is_first = false;

            try$(k_formatter.format(pair.m_key));
            try$(try_put_literal(": "sv));
            try$(v_formatter.format(pair.m_value));
        }
        try$(try_put_literal(" }"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename K, typename T>
class Formatter<OrderedMap<K, T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<OrderedMap<K, T>> {
        return Formatter<OrderedMap<K, T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<OrderedMap<K, T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(OrderedMap<K, T> const& ordered_map) -> ErrorOr<void> {
        try$(try_put_literal("{ "sv));

        bool is_first = true;
        for ( auto const& pair : ordered_map ) {
            auto k_formatter = Formatter<K>::new_from_format_applier(clone_format_applier());
            auto v_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());

            if ( !is_first )
                try$(try_put_literal(", "sv));
            else
                is_first = false;

            try$(k_formatter.format(pair.m_key));
            try$(try_put_literal(": "sv));
            try$(v_formatter.format(pair.m_value));
        }
        try$(try_put_literal(" }"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<Range<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<Range<T>> {
        return Formatter<Range<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Range<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Range<T> const& range) -> ErrorOr<void> {
        auto formatter = Formatter<T>::new_from_format_applier(clone_format_applier());

        try$(formatter.format(*range.begin()));
        try$(try_put_literal(".."sv));
        try$(formatter.format(*range.end()));

        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<RangeInclusive<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<RangeInclusive<T>> {
        return Formatter<RangeInclusive<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<RangeInclusive<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(RangeInclusive<T> const& range_inclusive) -> ErrorOr<void> {
        auto formatter = Formatter<T>::new_from_format_applier(clone_format_applier());

        try$(formatter.format(*range_inclusive.begin()));
        try$(try_put_literal("..="sv));
        try$(formatter.format(*range_inclusive.end() - 1));

        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<>
class Formatter<String> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<String>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<String>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(String const&) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<>
class Formatter<StringBuilder> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier) -> Formatter<StringBuilder>;
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder&, FormatParser::Result) -> Formatter<StringBuilder>;

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(StringBuilder const&) const -> ErrorOr<void>;

private:
    explicit Formatter(FormatApplier);
};

template<typename T>
class Formatter<Vector<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<Vector<T>> {
        return Formatter<Vector<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Vector<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Vector<T> const& vector) -> ErrorOr<void> {
        if ( display_as() == FormatParser::DisplayAs::Pointer ) {
            Formatter<T const*> formatter{ *this };
            try$(formatter.format(vector.raw_data()));

            return {};
        }

        try$(try_put_literal("[ "sv));

        bool is_first = true;
        for ( auto const& e : vector ) {
            auto e_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());
            if ( !is_first )
                try$(try_put_literal(", "sv));
            else
                is_first = false;

            try$(e_formatter.format(e));
        }
        try$(try_put_literal(" ]"sv));
        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T, typename E>
class Formatter<Result<T, E>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<Result<T, E>> {
        return Formatter<Result<T, E>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Result<T, E>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Result<T, E> const& result) -> ErrorOr<void> {
        if ( result.is_value() ) {
            auto v_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());

            try$(try_put_literal("Value("sv));
            try$(v_formatter.format(result.value()));
            try$(try_put_literal(")"sv));
        } else {
            auto e_formatter = Formatter<E>::new_from_format_applier(clone_format_applier());

            try$(try_put_literal("Error("sv));
            try$(e_formatter.format(result.error()));
            try$(try_put_literal(")"sv));
        }

        return {};
    }

private:
    explicit Formatter(FormatApplier format_applier)
        : FormatApplier{ Cxx::move(format_applier) } {
    }
};

template<typename T>
class Formatter<Option<T>> final : public FormatApplier {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_format_applier(FormatApplier format_applier) -> Formatter<Option<T>> {
        return Formatter<Option<T>>{ Cxx::move(format_applier) };
    }
    [[nodiscard]]
    static auto new_from_parser_result(StringBuilder& string_builder, FormatParser::Result result) -> Formatter<Option<T>> {
        return new_from_format_applier(FormatApplier::new_from_parser_result(string_builder, Cxx::move(result)));
    }

    ~Formatter() override = default;

    /**
     * @brief Performs the format on the given string-builder
     */
    auto format(Option<T> const& option) -> ErrorOr<void> {
        if ( option.is_present() ) {
            auto v_formatter = Formatter<T>::new_from_format_applier(clone_format_applier());

            try$(try_put_literal("Some("sv));
            try$(v_formatter.format(option.value()));
            try$(try_put_literal(")"sv));
        } else
            try$(try_put_literal("OptNone"sv));

        return {};
    }
};
