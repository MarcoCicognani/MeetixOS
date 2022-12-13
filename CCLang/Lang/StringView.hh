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

#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/Order.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>
#include <CCLang/Lang/Slice.hh>

#define as_string_view$(expression) #expression##sv

class StringView final {
public:
    enum class CaseSensible : bool {
        Yes,
        No
    };

    enum class KeepEmpty : bool {
        Yes,
        No
    };

    enum class IntBase {
        Binary  = 2,
        Octal   = 8,
        Decimal = 10,
        Hex     = 16
    };

    enum class ParseMode {
        Begin,
        TrimWhitesAndBegin,
        BeginToEnd,
        TrimWhitesAndBeginToEnd,
    };

    enum class TrimMode {
        Left,
        Right,
        Both
    };

    using ConstIterator               = Slice<char const>::ConstIterator;
    using ConstReverseIterator        = Slice<char const>::ConstReverseIterator;
    using ConstReverseIteratorWrapper = Slice<char const>::ConstReverseIteratorWrapper;

public:
    /**
     * @brief Error safe factory function
     */
    static auto from_cstr(char const* c_str) -> StringView;
    static auto from_raw_parts(char const* c_str, usize len) -> StringView;

    /**
     * @brief Constructors
     */
    explicit(false) StringView()                      = default;
    explicit(false) StringView(StringView const& rhs) = default;
    explicit(false) StringView(StringView&& rhs);

    ~StringView() = default;

    auto operator=(StringView const& rhs) -> StringView&;
    auto operator=(StringView&& rhs) -> StringView&;

    /**
     * @brief Swaps this string view with another
     */
    auto swap(StringView& rhs) -> void;

    /**
     * @brief Access operators
     */
    auto at(usize index) const -> char const&;
    auto operator[](usize index) const -> char const&;

    /**
     * @brief Compares this string view with another
     */
    auto compare(StringView rhs) const -> Order;
    auto equals_ignore_case(StringView rhs) const -> bool;

    /**
     * @brief Returns a sub_string_view of this string view
     */
    auto sub_string_view(usize start) const -> StringView;
    auto sub_string_view(usize start, usize count) const -> StringView;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    auto trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> StringView;
    auto trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const -> StringView;

    /**
     * @brief Returns whether the given rhs is at the start of this string view
     */
    auto starts_with(StringView rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;
    auto starts_with(char rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;

    auto ends_with(StringView rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;
    auto ends_with(char rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;

    /**
     * @brief Converts this StringView into an integer
     */
    template<typename T = i32>
    auto as_int(IntBase int_base = IntBase::Decimal, ParseMode parse_mode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    template<typename T = u32>
    auto as_uint(IntBase int_base = IntBase::Decimal, ParseMode parse_mode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    auto find(char needle, usize start = 0) const -> Option<usize>;
    auto find(StringView needle, usize start = 0) const -> Option<usize>;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    auto find_last(char needle) const -> Option<usize>;
    auto find_last_if(Predicate<char> auto predicate) const -> Option<usize> {
        for ( auto const i : usize::range(len() - 1, 0).reverse_iter() ) {
            if ( predicate(at(i)) ) {
                return i;
            }
        }
        return {};
    }

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    auto find_all(StringView needle) const -> Vector<usize>;
    auto try_find_all(StringView needle) const -> ErrorOr<Vector<usize>>;

    /**
     * @brief Splits this StringView and puts the split_view values into a vector or pass them to a predicate
     */
    auto split_view(char separator, KeepEmpty keep_empty = KeepEmpty::No) const -> Vector<StringView>;
    auto split_view(StringView separator, KeepEmpty keep_empty = KeepEmpty::No) const -> Vector<StringView>;

    auto try_split_view(char separator, KeepEmpty keep_empty = KeepEmpty::No) const -> ErrorOr<Vector<StringView>>;
    auto try_split_view(StringView separator, KeepEmpty keep_empty = KeepEmpty::No) const -> ErrorOr<Vector<StringView>>;

    /**
     * @brief Comparison operators
     */
    auto operator==(StringView const& rhs) const -> bool;
    auto operator!=(StringView const& rhs) const -> bool;
    auto operator<(StringView const& rhs) const -> bool;
    auto operator<=(StringView const& rhs) const -> bool;
    auto operator>(StringView const& rhs) const -> bool;
    auto operator>=(StringView const& rhs) const -> bool;

    /**
     * @brief Returns whether the given rhs is contained into this StringView
     */
    auto contains(StringView rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;
    auto contains(char rhs, CaseSensible case_sensible = CaseSensible::Yes) const -> bool;

    /**
     * @brief For-each support
     */
    auto begin() const -> ConstIterator;
    auto end() const -> ConstIterator;

    /**
     * @brief Reverse for-each support
     */
    auto rbegin() const -> ConstReverseIterator;
    auto rend() const -> ConstReverseIterator;
    auto reverse_iter() const -> ConstReverseIteratorWrapper;

    /**
     * @brief Slice
     */
    auto as_slice() const -> Slice<char const>;

    /**
     * @brief Hashing support
     */
    auto hash_code() const -> usize;

    /**
     * @brief Getters
     */
    auto as_cstr() const -> char const*;
    auto len() const -> usize;

    auto is_empty() const -> bool;
    auto is_null() const -> bool;
    auto is_null_or_empty() const -> bool;

private:
    explicit StringView(char const* c_str, usize count);

private:
    Slice<char const> m_chars_slice = Slice<char const>::empty();
};

auto operator""sv(char const* c_str, __SIZE_TYPE__ len) -> StringView {
    return StringView::from_raw_parts(c_str, len);
}

template<>
struct TypeTraits<StringView> final : public Details::TypeTraits<StringView> {
    static auto hash(StringView const& string_view) -> usize {
        return string_view.hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true; /* since this type is a read-only view of a slice the destructor does nothing */
    }
};

namespace Cxx {

auto swap(StringView& lhs, StringView& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
