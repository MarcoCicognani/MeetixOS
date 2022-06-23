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
#include <LibTC/Trait/IsIntegral.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Collection {
namespace Details {

template<Integral T>
class RangeIterator {
public:
    /**
     * @brief Constructor
     */
    explicit constexpr RangeIterator(T first)
        : m_current{ first } {
    }

    RangeIterator(RangeIterator const&) = default;

    RangeIterator& operator=(RangeIterator const&) = default;

    /**
     * @brief Increment operators
     */
    [[gnu::always_inline]] RangeIterator& operator++() {
        ++m_current;
        return *this;
    }
    [[gnu::always_inline]] RangeIterator operator++(int) {
        RangeIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    [[gnu::always_inline]] T operator*() const {
        return m_current;
    }

    /**
     * @brief Getters
     */
    T value() const {
        return m_current;
    }

    /**
     * @brief Comparison operator
     */
    [[nodiscard]] inline bool operator<=>(RangeIterator const&) const = default;

private:
    T m_current;
};

} /* namespace Details */

template<Integral T>
class Range {
public:
    using Iterator = Details::RangeIterator<T>;

public:
    /**
     * @brief Constructor
     */
    constexpr Range(T first, T end)
        : m_first{ first }
        , m_end{ end } {
    }

    /**
     * @brief Iterator
     */
    [[gnu::always_inline]] Iterator begin() const {
        return Iterator{ m_first };
    }
    [[gnu::always_inline]] Iterator end() const {
        return Iterator{ m_end };
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
    constexpr RangeInclusive(T first, T last)
        : Range<T>{ first, last + 1 } {
    }
};

} /* namespace Collection */

using Collection::Range;
using Collection::RangeInclusive;

namespace Trait {

template<typename T>
struct TypeIntrinsics<Range<T>> : public Details::TypeIntrinsics<Range<T>> {
    static constexpr bool equals(Range<T> const& a, Range<T> const& b) {
        return TypeIntrinsics<T>::equals(a.begin().value(), b.begin().value) && TypeIntrinsics<T>::equals(a.end().value(), b.end().value());
    }

    static constexpr bool is_trivial() {
        return true;
    }
};

template<typename T>
struct TypeIntrinsics<RangeInclusive<T>> : public Details::TypeIntrinsics<RangeInclusive<T>> {
    static constexpr bool equals(Range<T> const& a, Range<T> const& b) {
        return TypeIntrinsics<T>::equals(a.begin().value(), b.begin().value) && TypeIntrinsics<T>::equals(a.end().value(), b.end().value());
    }

    static constexpr bool is_trivial() {
        return true;
    }
};

} /* namespace Trait */
} /* namespace TC */