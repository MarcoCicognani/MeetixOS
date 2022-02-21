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

#include <LibTC/Collection/Vector.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Collection {

class String;

enum class TrimWhitespace {
    Yes,
    No
};

enum class TrimMode {
    Left,
    Right,
    Both
};

enum class CaseSensitivity {
    Sensitive,
    Insensitive
};

class StringView {
public:
    using ConstIterator = Details::VectorIterator<StringView const, char const>;

public:
    /**
     * @brief Constructors
     */
    constexpr StringView() = default;
    constexpr StringView(char const* str, usize count)
        : m_chars_ptr{ str }
        , m_chars_count{ count } {}
    StringView(String const& string);
    StringView(char const* str);
    StringView(StringView const& rhs);
    StringView(StringView&& rhs) noexcept;
    ~StringView() = default;

    StringView& operator=(String const& str);
    StringView& operator=(char const* str);
    StringView& operator=(StringView const& rhs);
    StringView& operator=(StringView&& rhs) noexcept;

    /**
     * @brief Swaps this string view with another
     */
    void swap(StringView& rhs) noexcept;

    /**
     * @brief Access operators
     */
    [[nodiscard]] char const& at(usize index) const;
    [[nodiscard]] char const& operator[](usize index) const;

    /**
     * @brief Compares this string view with another
     */
    [[nodiscard]] int  compare(StringView rhs) const;
    [[nodiscard]] bool equals_ignore_case(StringView rhs) const;

    /**
     * @brief Returns a sub_string of this string view
     */
    [[nodiscard]] StringView sub_string(usize start) const;
    [[nodiscard]] StringView sub_string(usize start, usize count) const;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    [[nodiscard]] StringView trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const;
    [[nodiscard]] StringView trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const;

    /**
     * @brief Returns whether the given rhs is at the start of this string view
     */
    [[nodiscard]] bool starts_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;
    [[nodiscard]] bool starts_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;

    [[nodiscard]] bool ends_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;
    [[nodiscard]] bool ends_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;

    /**
     * @brief Converts this StringView into an integer
     */
    template<typename T = i32>
    [[nodiscard]] Option<T> as_int(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const;

    template<typename T = u32>
    [[nodiscard]] Option<T> as_uint(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const;

    template<typename T = u32>
    [[nodiscard]] Option<T> as_uint_from_hex(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const;

    template<typename T = u32>
    [[nodiscard]] Option<T> as_uint_from_octal(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    [[nodiscard]] Option<usize> find(char needle, size_t start = 0) const;
    [[nodiscard]] Option<usize> find(StringView needle, size_t start = 0) const;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    [[nodiscard]] Option<usize> find_last(char needle) const;

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    [[nodiscard]] Vector<usize>          find_all(StringView needle) const;
    [[nodiscard]] ErrorOr<Vector<usize>> try_find_all(StringView needle) const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(StringView const& rhs) const;
    [[nodiscard]] bool operator!=(StringView const& rhs) const;
    [[nodiscard]] bool operator<(StringView const& rhs) const;
    [[nodiscard]] bool operator<=(StringView const& rhs) const;
    [[nodiscard]] bool operator>(StringView const& rhs) const;
    [[nodiscard]] bool operator>=(StringView const& rhs) const;

    /**
     * @brief Returns whether the given rhs is contained into this StringView
     */
    [[nodiscard]] bool contains(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;
    [[nodiscard]] bool contains(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;

    /**
     * @brief For-each support
     */
    ConstIterator begin() const;
    ConstIterator end() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] char const* as_cstr() const;
    [[nodiscard]] usize       len() const;
    [[nodiscard]] usize       count() const;

    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool is_null() const;
    [[nodiscard]] bool is_null_or_empty() const;

private:
    char const* m_chars_ptr{ nullptr };
    usize       m_chars_count{ 0 };
};

} /* namespace Collection */

using Collection::CaseSensitivity;
using Collection::StringView;
using Collection::TrimMode;
using Collection::TrimWhitespace;

} /* namespace TC */

[[nodiscard]] constexpr inline TC::StringView operator"" sv(char const* c_str, usize len) {
    return TC::StringView{ c_str, len };
}
