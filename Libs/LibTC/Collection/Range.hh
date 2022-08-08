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
#include <LibTC/Collection/ReverseIteratorSupport.hh>
#include <LibTC/Forward.hh>
#include <LibTC/Meta.hh>
#include <LibTC/TypeTraits.hh>

namespace TC {
namespace Collection {
namespace Details {

template<Integral T, bool IsReverse>
class RangeIterator {
public:
    /**
     * @brief Constructor
     */
    explicit constexpr RangeIterator(T first)
        : m_current{ first } {
    }

    RangeIterator(RangeIterator const&) = default;

    auto operator=(RangeIterator const&) -> RangeIterator& = default;

    /**
     * @brief Increment operators
     */
    [[gnu::always_inline]] auto operator++() -> RangeIterator& {
        if constexpr ( IsReverse ) {
            --m_current;
        } else {
            ++m_current;
        }
        return *this;
    }
    [[gnu::always_inline]] auto operator++(int) -> RangeIterator {
        RangeIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    [[gnu::always_inline]] auto operator*() const -> T {
        return m_current;
    }

    /**
     * @brief Getters
     */
    auto value() const -> T {
        return m_current;
    }

    /**
     * @brief Comparison operator
     */
    [[nodiscard]] inline auto operator<=>(RangeIterator const&) const -> bool = default;

private:
    T m_current;
};

} /* namespace Details */

template<Integral T>
class Range {
public:
    using Iterator        = Details::RangeIterator<T, false>;
    using ReverseIterator = Details::RangeIterator<T, true>;

public:
    /**
     * @brief Constructor
     */
    explicit constexpr Range(T first, T end) noexcept
        : m_first{ first }
        , m_end{ end } {
    }

    /**
     * @brief IteratorProvider
     */
    [[gnu::always_inline]] auto begin() const -> Iterator {
        return Iterator{ m_first };
    }
    [[gnu::always_inline]] auto end() const -> Iterator {
        return Iterator{ m_end };
    }

    [[gnu::always_inline]] auto rbegin() const -> Iterator {
        return ReverseIterator{ m_end };
    }
    [[gnu::always_inline]] auto rend() const -> Iterator {
        return ReverseIterator{ m_first };
    }

    auto reverse_iter() const -> ReverseIteratorSupport::Wrapper<Range<T>> {
        return ReverseIteratorSupport::in_reverse(*this);
    }

private:
    T m_first;
    T m_end;
};

template<Integral T>
class RangeInclusive : public Range<T> {
public:
    /**
     * @brief Constructor
     */
    explicit constexpr RangeInclusive(T first, T last) noexcept
        : Range<T>{ first, last + 1 } {
    }
};

} /* namespace Collection */

using Collection::Range;
using Collection::RangeInclusive;

template<typename T>
struct TypeTraits<Range<T>> : public Details::TypeTraits<Range<T>> {
    static constexpr auto equals(Range<T> const& a, Range<T> const& b) -> bool {
        return TypeTraits<T>::equals(a.begin().value(), b.begin().value)
            && TypeTraits<T>::equals(a.end().value(), b.end().value());
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<typename T>
struct TypeTraits<RangeInclusive<T>> : public Details::TypeTraits<RangeInclusive<T>> {
    static constexpr auto equals(Range<T> const& a, Range<T> const& b) -> bool {
        return TypeTraits<T>::equals(a.begin().value(), b.begin().value)
            && TypeTraits<T>::equals(a.end().value(), b.end().value());
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

} /* namespace TC */