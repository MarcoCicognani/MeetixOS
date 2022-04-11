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

namespace TC {
namespace Collection {

template<typename T>
    requires IsIntegral<T>
class RangeIterator {
public:
    /**
     * @brief Constructor
     */
    RangeIterator(T first, T end, T step)
        : m_current{ first }
        , m_end{ end }
        , m_step{ step } {
    }

    RangeIterator(RangeIterator const&) = default;

    RangeIterator& operator=(RangeIterator const&) = default;

    /**
     * @brief Increment operators
     */
    RangeIterator& operator++() {
        if ( m_current < m_end && m_current + m_step < m_end )
            m_current += m_step;
        else
            m_current = m_end;
        return *this;
    }
    RangeIterator operator++(int) {
        RangeIterator it{ *this };
                      operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    T operator*() const {
        VERIFY_FALSE(is_end());
        return m_current;
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] bool is_end() const {
        return m_current == m_end;
    }

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(RangeIterator const& rhs) const {
        return m_current == rhs.m_current;
    }
    [[nodiscard]] bool operator!=(RangeIterator const& rhs) const {
        return m_current != rhs.m_current;
    }
    [[nodiscard]] bool operator<(RangeIterator const& rhs) const {
        return m_current < rhs.m_current;
    }
    [[nodiscard]] bool operator>(RangeIterator const& rhs) const {
        return m_current > rhs.m_current;
    }
    [[nodiscard]] bool operator<=(RangeIterator const& rhs) const {
        return m_current <= rhs.m_current;
    }
    [[nodiscard]] bool operator>=(RangeIterator const& rhs) const {
        return m_current >= rhs.m_current;
    }

private:
    T m_current;
    T m_end;
    T m_step;
};

template<typename T>
    requires IsIntegral<T>
class Range {
public:
    using Iterator = RangeIterator<T>;

public:
    /**
     * @brief Constructor
     */
    explicit constexpr Range(T first, T end, T step)
        : m_first{ first }
        , m_end{ end }
        , m_step{ step } {
    }

    /**
     * @brief Getters
     */
    T first() const {
        return m_first;
    }
    T last() const {
        return m_end - 1;
    }
    T step() const {
        return m_step;
    }

    /**
     * @brief Iterator
     */
    Iterator begin() const {
        return RangeIterator{ m_first, m_end, m_step };
    }
    Iterator end() const {
        return RangeIterator{ m_end, m_end, m_step };
    }

private:
    T m_first;
    T m_end;
    T m_step;
};

template<typename T>
constexpr Range<T> iter_range(T first, T end, T step = 1) {
    return Range<T>{ first, end, step };
}

template<typename T>
constexpr Range<T> iter_range_inclusive(T first, T last, T step = 1) {
    return iter_range(first, last + 1, step);
}

} /* namespace Collection */

using Collection::iter_range;
using Collection::iter_range_inclusive;
using Collection::Range;

} /* namespace TC */