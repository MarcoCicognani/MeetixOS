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

#pragma once

#include <LibTC/Collection/Enums/CaseSensitivity.hh>
#include <LibTC/Collection/Enums/TrimMode.hh>
#include <LibTC/Collection/Enums/TrimWhitespace.hh>
#include <LibTC/Collection/Vector.hh>
#include <LibTC/Forward.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/TypeTraits.hh>

namespace TC {
namespace Collection {

class StringView {
public:
    using ConstIterator = Details::VectorIterator<StringView const, char const, false>;

public:
    /**
     * @brief Constructors
     */
    constexpr explicit(false) StringView() = default;
    constexpr explicit(false) StringView(char const* c_str, usize count) noexcept
        : m_chars_ptr{ c_str }
        , m_chars_count{ count } {
    }
    constexpr explicit(false) StringView(StringView const& rhs) noexcept
        : StringView{ rhs.m_chars_ptr, rhs.m_chars_count } {
    }
    constexpr explicit(false) StringView(StringView&& rhs) noexcept
        : m_chars_ptr{ Cxx::exchange(rhs.m_chars_ptr, nullptr) }
        , m_chars_count{ Cxx::exchange(rhs.m_chars_count, 0) } {
    }
    ~StringView() = default;

    auto operator=(StringView const& rhs) -> StringView&;
    auto operator=(StringView&& rhs) noexcept -> StringView&;

    /**
     * @brief Swaps this string view with another
     */
    auto swap(StringView& rhs) noexcept -> void;

    /**
     * @brief Access operators
     */
    [[nodiscard]] auto at(usize index) const -> char const&;
    [[nodiscard]] auto operator[](usize index) const -> char const&;

    /**
     * @brief Compares this string view with another
     */
    [[nodiscard]] auto compare(StringView rhs) const -> int;
    [[nodiscard]] auto equals_ignore_case(StringView rhs) const -> bool;

    /**
     * @brief Returns a sub_string_view of this string view
     */
    [[nodiscard]] auto sub_string_view(usize start) const -> StringView;
    [[nodiscard]] auto sub_string_view(usize start, usize count) const -> StringView;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    [[nodiscard]] auto trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> StringView;
    [[nodiscard]] auto trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const -> StringView;

    /**
     * @brief Returns whether the given rhs is at the start of this string view
     */
    [[nodiscard]] auto starts_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;
    [[nodiscard]] auto starts_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;

    [[nodiscard]] auto ends_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;
    [[nodiscard]] auto ends_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;

    /**
     * @brief Converts this StringView into an integer
     */
    template<typename T = i32>
    [[nodiscard]] auto as_int(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const -> Option<T>;

    template<typename T = u32>
    [[nodiscard]] auto as_uint(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const -> Option<T>;

    template<typename T = u32>
    [[nodiscard]] auto as_uint_from_hex(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const -> Option<T>;

    template<typename T = u32>
    [[nodiscard]] auto as_uint_from_octal(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const -> Option<T>;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    [[nodiscard]] auto find(char needle, size_t start = 0) const -> Option<usize>;
    [[nodiscard]] auto find(StringView needle, size_t start = 0) const -> Option<usize>;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    [[nodiscard]] auto find_last(char needle) const -> Option<usize>;

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    [[nodiscard]] auto find_all(StringView needle) const -> Vector<usize>;
    [[nodiscard]] auto try_find_all(StringView needle) const -> ErrorOr<Vector<usize>>;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] auto operator==(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator!=(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator<(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator<=(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator>(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator>=(StringView const& rhs) const -> bool;

    /**
     * @brief Returns whether the given rhs is contained into this StringView
     */
    [[nodiscard]] auto contains(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;
    [[nodiscard]] auto contains(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;

    /**
     * @brief For-each support
     */
    auto begin() const -> ConstIterator;
    auto end() const -> ConstIterator;

    /**
     * @brief Getters
     */
    [[nodiscard]] auto as_cstr() const -> char const*;
    [[nodiscard]] auto len() const -> usize;
    [[nodiscard]] auto count() const -> usize;

    [[nodiscard]] auto is_empty() const -> bool;
    [[nodiscard]] auto is_null() const -> bool;
    [[nodiscard]] auto is_null_or_empty() const -> bool;

private:
    char const* m_chars_ptr{ nullptr };
    usize       m_chars_count{ 0 };
};

} /* namespace Collection */

using Collection::StringView;

template<>
struct TypeTraits<StringView> : public Details::TypeTraits<StringView> {
    static auto hash(StringView const& value) -> usize {
        return Hashing::string_calculate_hash(value.as_cstr(), value.len());
    }

    static constexpr auto is_trivial() -> bool {
        return true; /* since this type is a read-only view of a slice the destructor does nothing */
    }
};

} /* namespace TC */

[[nodiscard]] constexpr auto operator""sv(char const* c_str, usize len) noexcept -> TC::StringView {
    return TC::StringView{ c_str, len };
}
