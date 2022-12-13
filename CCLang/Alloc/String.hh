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

#include <CCLang/Forward.hh>

#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Alloc/StringStorage.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>
#include <CCLang/Lang/StringView.hh>

class String final : public DenyCopy {
public:
    using ConstIterator               = StringView::ConstIterator;
    using ConstReverseIterator        = StringView::ConstReverseIterator;
    using ConstReverseIteratorWrapper = StringView::ConstReverseIteratorWrapper;

    using CaseSensible = StringView::CaseSensible;
    using KeepEmpty    = StringView::KeepEmpty;
    using IntBase      = StringView::IntBase;
    using ParseMode    = StringView::ParseMode;
    using TrimMode     = StringView::TrimMode;

public:
    /**
     * @brief Non-Error safe factory functions
     */
    static auto empty() -> String;
    static auto from_other(String const& rhs) -> String;
    static auto from_view(StringView string_view) -> String;

    /**
     * @brief Error safe Factory functions
     */
    static auto try_empty() -> ErrorOr<String>;
    static auto try_from_other(String const& rhs) -> ErrorOr<String>;
    static auto try_from_view(StringView string_view) -> ErrorOr<String>;

    /**
     * @brief Move constructor and move assignment
     */
    String(String&& rhs);
    auto operator=(String&& rhs) -> String&;

    ~String() = default;

    /**
     * @brief Cloning only increments strong references to the memory
     */
    auto clone() const -> String;

    /**
     * @brief Swaps this string with another
     */
    auto swap(String& rhs) -> void;

    /**
     * @brief Access operators
     */
    auto at(usize index) const -> char const&;
    auto operator[](usize index) const -> char const&;

    /**
     * @brief Compares this string with another
     */
    auto compare(StringView rhs) const -> Order;
    auto equals_ignore_case(StringView rhs) const -> bool;

    /**
     * @brief Returns a sub_string_view of this string
     */
    auto sub_string(usize start) const -> String;
    auto sub_string(usize start, usize count) const -> String;

    auto try_sub_string(usize start) const -> ErrorOr<String>;
    auto try_sub_string(usize start, usize count) const -> ErrorOr<String>;

    auto sub_string_view(usize start) const -> StringView;
    auto sub_string_view(usize start, usize count) const -> StringView;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    auto trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> String;
    auto try_trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> ErrorOr<String>;

    auto trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const -> String;
    auto try_trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const -> ErrorOr<String>;

    auto trim_view(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> StringView;
    auto trim_whitespaces_view(TrimMode trim_mode = TrimMode::Both) const -> StringView;

    /**
     * @brief Returns whether the given rhs is at the start of this string
     */
    auto starts_with(StringView rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;
    auto starts_with(char rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;

    auto ends_with(StringView rhs, CaseSensible case_sensible= CaseSensible::Yes) const -> bool;
    auto ends_with(char rhs, CaseSensible case_sensible= CaseSensible::Yes) const -> bool;

    /**
     * @brief Converts this String into an integer
     */
    template<typename T = i32>
    auto as_int(IntBase int_base = IntBase::Decimal, ParseMode parse_mode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    template<typename T = u32>
    auto as_uint(IntBase int_base = IntBase::Decimal, ParseMode parse_mode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    auto find(char rhs, usize start = 0) const -> Option<usize>;
    auto find(StringView rhs, usize start = 0) const -> Option<usize>;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    auto find_last(char rhs) const -> Option<usize>;

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    auto find_all(StringView needle) const -> Vector<usize>;
    auto try_find_all(StringView needle) const -> ErrorOr<Vector<usize>>;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    auto to_lowercase() const -> String;
    auto try_to_lowercase() const -> ErrorOr<String>;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    auto to_uppercase() const -> String;
    auto try_to_uppercase() const -> ErrorOr<String>;

    /**
     * @brief Returns a new string with the reverse of this one
     */
    auto to_reverse() const -> String;
    auto try_to_reverse() const -> ErrorOr<String>;

    /**
     * @brief Comparison operators
     */
    auto operator==(String const& rhs) const -> bool;
    auto operator==(StringView const& rhs) const -> bool;
    auto operator!=(String const& rhs) const -> bool;
    auto operator!=(StringView const& rhs) const -> bool;
    auto operator<(String const& rhs) const -> bool;
    auto operator<(StringView const& rhs) const -> bool;
    auto operator<=(String const& rhs) const -> bool;
    auto operator<=(StringView const& rhs) const -> bool;
    auto operator>(String const& rhs) const -> bool;
    auto operator>(StringView const& rhs) const -> bool;
    auto operator>=(String const& rhs) const -> bool;
    auto operator>=(StringView const& rhs) const -> bool;

    /**
     * @brief Returns whether the given rhs is contained into this String
     */
    auto contains(StringView needle, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;
    auto contains(char needle, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;

    /**
     * @brief For-each support
     */
    auto begin() const -> ConstIterator;
    auto end() const -> ConstIterator;

    /**
     * @brief For-each support
     */
    auto rbegin() const -> ConstReverseIterator;
    auto rend() const -> ConstReverseIterator;
    auto reverse_iter() const -> ConstReverseIteratorWrapper;

    /**
     * @brief Hashing support
     */
    auto hash_code() const -> usize;

    /**
     * @brief Getters
     */
    auto as_cstr() const -> char const*;
    auto len() const -> usize;
    auto count() const -> usize;

    auto is_empty() const -> bool;

    auto as_string_view() const -> StringView;

private:
    explicit String(NonNullRef<StringStorage>);

private:
    NonNullRef<StringStorage> m_string_storage_ref;
};

template<>
struct TypeTraits<String> final : public Details::TypeTraits<String> {
    static auto hash(String const& string) -> usize {
        return string.hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }
};

namespace Cxx {

auto swap(String& lhs, String& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
