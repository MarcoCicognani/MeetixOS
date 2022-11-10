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

#include <ST/Forward.hh>

#include <ST/Core/Assertions.hh>
#include <ST/Core/Meta.hh>
#include <ST/Core/TypeTraits.hh>
#include <ST/Lang/ReverseIteratorSupport.hh>

namespace Details {

template<Integral T, bool IsReverse>
class RangeIterator final {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_current(T current) -> RangeIterator<T, IsReverse> {
        return RangeIterator<T, IsReverse>{ current };
    }

    RangeIterator(RangeIterator const&)                    = default;
    auto operator=(RangeIterator const&) -> RangeIterator& = default;

    /**
     * @brief Increment operators
     */
    [[gnu::always_inline]]
    auto operator++() -> RangeIterator& {
        if constexpr ( IsReverse ) {
            --m_current;
        } else {
            ++m_current;
        }
        return *this;
    }
    [[gnu::always_inline]]
    auto operator++(int) -> RangeIterator {
        RangeIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    [[gnu::always_inline]]
    auto operator*() const -> T {
        return m_current;
    }

    /**
     * @brief Comparison operator
     */
    [[nodiscard]]
    inline auto operator<=>(RangeIterator const&) const -> bool = default;

private:
    explicit constexpr RangeIterator(T current)
        : m_current{ current } {
    }

private:
    T m_current;
};

} /* namespace Details */

template<Integral T>
class Range final {
public:
    using Iterator               = Details::RangeIterator<T, false>;
    using ReverseIterator        = Details::RangeIterator<T, true>;
    using ReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<Range<T>>;

public:
    /**
     * @brief Constructor
     */
    explicit constexpr Range(T first, T end)
        : m_first{ first }
        , m_end{ end } {
    }

    /**
     * @brief IteratorProvider
     */
    [[gnu::always_inline]]
    auto begin() const -> Iterator {
        return Iterator::new_from_current(m_first);
    }
    [[gnu::always_inline]]
    auto end() const -> Iterator {
        return Iterator::new_from_current(m_end);
    }

    [[gnu::always_inline]]
    auto rbegin() const -> Iterator {
        return ReverseIterator::new_from_current(m_end);
    }
    [[gnu::always_inline]]
    auto rend() const -> Iterator {
        return ReverseIterator::new_from_current(m_first);
    }

    auto reverse_iter() const -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

private:
    T m_first;
    T m_end;
};

template<Integral T>
class RangeInclusive final {
public:
    using Iterator               = Details::RangeIterator<T, false>;
    using ReverseIterator        = Details::RangeIterator<T, true>;
    using ReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<Range<T>>;

public:
    /**
     * @brief Constructor
     */
    explicit constexpr RangeInclusive(T first, T end)
        : m_first{ first }
        , m_end{ end + 1 } {
    }

    /**
     * @brief IteratorProvider
     */
    [[gnu::always_inline]]
    auto begin() const -> Iterator {
        return Iterator::new_from_current(m_first);
    }
    [[gnu::always_inline]]
    auto end() const -> Iterator {
        return Iterator::new_from_current(m_end);
    }

    [[gnu::always_inline]]
    auto rbegin() const -> Iterator {
        return ReverseIterator::new_from_current(m_end);
    }
    [[gnu::always_inline]]
    auto rend() const -> Iterator {
        return ReverseIterator::new_from_current(m_first);
    }

    auto reverse_iter() const -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

private:
    T m_first;
    T m_end;
};

template<typename T>
struct TypeTraits<Range<T>> final : public Details::TypeTraits<Range<T>> {
    static constexpr auto equals(Range<T> const& a, Range<T> const& b) -> bool {
        return TypeTraits<T>::equals(a.begin().value(), b.begin().value)
            && TypeTraits<T>::equals(a.end().value(), b.end().value());
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<typename T>
struct TypeTraits<RangeInclusive<T>> final : public Details::TypeTraits<RangeInclusive<T>> {
    static constexpr auto equals(Range<T> const& a, Range<T> const& b) -> bool {
        return TypeTraits<T>::equals(a.begin().value(), b.begin().value)
            && TypeTraits<T>::equals(a.end().value(), b.end().value());
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};
