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
#include <LibTC/Collection/StringStorage.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Collection {

class String {
public:
    using ConstIterator = StringView::ConstIterator;

public:
    /**
     * @brief Error Safe Constructors
     */
    static ErrorOr<String> try_construct_from(StringView string_view);
    static ErrorOr<String> try_construct_from(char const* str, usize count);

    /**
     * @brief Constructors
     */
    String();
    String(char const* str);
    String(char const* str, usize count);
    String(StringView string_view);
    String(String const& rhs) = default;
    String(String&& rhs) noexcept;
    ~String() = default;

    String& operator=(char const* rhs);
    String& operator=(StringView rhs);
    String& operator=(String const& rhs);
    String& operator=(String&& rhs) noexcept;

    /**
     * @brief Swaps this string with another
     */
    void swap(String& rhs) noexcept;

    /**
     * @brief Access operators
     */
    [[nodiscard]] char const& at(usize index) const;
    [[nodiscard]] char const& operator[](usize index) const;

    /**
     * @brief Compares this string with another
     */
    [[nodiscard]] int  compare(StringView rhs) const;
    [[nodiscard]] bool equals_ignore_case(StringView rhs) const;

    /**
     * @brief Returns a sub_string_view of this string
     */
    [[nodiscard]] String sub_string(usize start) const;
    [[nodiscard]] String sub_string(usize start, usize count) const;

    [[nodiscard]] ErrorOr<String> try_sub_string(usize start) const;
    [[nodiscard]] ErrorOr<String> try_sub_string(usize start, usize count) const;

    [[nodiscard]] StringView sub_string_view(usize start) const;
    [[nodiscard]] StringView sub_string_view(usize start, usize count) const;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    [[nodiscard]] String          trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const;
    [[nodiscard]] ErrorOr<String> try_trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const;

    [[nodiscard]] String          trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const;
    [[nodiscard]] ErrorOr<String> try_trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const;

    [[nodiscard]] StringView trim_view(StringView chars, TrimMode trim_mode = TrimMode::Both) const;
    [[nodiscard]] StringView trim_whitespaces_view(TrimMode trim_mode = TrimMode::Both) const;

    /**
     * @brief Returns whether the given rhs is at the start of this string
     */
    [[nodiscard]] bool starts_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;
    [[nodiscard]] bool starts_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;

    [[nodiscard]] bool ends_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;
    [[nodiscard]] bool ends_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const;

    /**
     * @brief Converts this String into an integer
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
     * @brief Returns a new string with the lowercase of this one
     */
    [[nodiscard]] String          to_lowercase() const;
    [[nodiscard]] ErrorOr<String> try_to_lowercase() const;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    [[nodiscard]] String          to_uppercase() const;
    [[nodiscard]] ErrorOr<String> try_to_uppercase() const;

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
     * @brief Returns whether the given rhs is contained into this String
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

    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] StringView as_string_view() const;

private:
    explicit String(NonNullRef<StringStorage>&& string_storage);

private:
    NonNullRef<StringStorage> m_string_storage_ref;
};

} /* namespace Collection */

using Collection::String;

namespace Trait {

template<>
struct TypeIntrinsics<String> : public Details::TypeIntrinsics<String> {
    static usize hash(String const& value) {
        return Hashing::string_calculate_hash(value.as_cstr(), value.len());
    }

    static constexpr bool is_trivial() {
        return false;
    }
};

} /* namespace Trait */
} /* namespace TC */
