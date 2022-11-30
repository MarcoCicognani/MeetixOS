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

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>

namespace Details {

template<typename T, bool IsReverse>
class RangeIterator final {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static constexpr auto from_current(T current) -> RangeIterator<T, IsReverse> {
        return RangeIterator<T, IsReverse>(current);
    }

    RangeIterator(RangeIterator&&)      = default;
    RangeIterator(RangeIterator const&) = default;

    auto operator=(RangeIterator&&) -> RangeIterator&      = default;
    auto operator=(RangeIterator const&) -> RangeIterator& = default;

    /**
     * @brief Increment operators
     */
    [[gnu::always_inline]]
    auto operator++() -> RangeIterator& {
        if constexpr ( IsReverse ) {
            --m_current_value;
        } else {
            ++m_current_value;
        }
        return *this;
    }
    [[gnu::always_inline]]
    auto operator++(int) -> RangeIterator {
        auto __prev = *this;

        operator++();
        return __prev;
    }

    /**
     * @brief ValueReference access operators
     */
    [[gnu::always_inline]]
    auto operator*() const -> T {
        return m_current_value;
    }

    /**
     * @brief Comparison operator
     */
    constexpr auto operator<=>(RangeIterator const&) const -> bool = default;

private:
    explicit constexpr RangeIterator(T current)
        : m_current_value(current) {
    }

private:
    T m_current_value;
};

} /* namespace Details */

template<typename T>
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
        : m_first(first)
        , m_end(end) {
    }

    /**
     * @brief IteratorProvider
     */
    constexpr auto begin() const -> Iterator {
        return Iterator::from_current(m_first);
    }
    constexpr auto end() const -> Iterator {
        return Iterator::from_current(m_end);
    }

    constexpr auto rbegin() const -> Iterator {
        return ReverseIterator::from_current(m_end);
    }
    constexpr auto rend() const -> Iterator {
        return ReverseIterator::from_current(m_first);
    }

    constexpr auto reverse_iter() const -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

private:
    T m_first;
    T m_end;
};

template<typename T>
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
        : m_first(first)
        , m_end(end + 1) {
    }

    /**
     * @brief IteratorProvider
     */
    constexpr auto begin() const -> Iterator {
        return Iterator::from_current(m_first);
    }
    constexpr auto end() const -> Iterator {
        return Iterator::from_current(m_end);
    }

    constexpr auto rbegin() const -> Iterator {
        return ReverseIterator::from_current(m_end);
    }
    constexpr auto rend() const -> Iterator {
        return ReverseIterator::from_current(m_first);
    }

    constexpr auto reverse_iter() const -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

private:
    T m_first;
    T m_end;
};

template<typename T>
struct TypeTraits<Range<T>> final : public Details::TypeTraits<Range<T>> {
    static constexpr auto equals(Range<T> const& a, Range<T> const& b) -> bool {
        return TypeTraits<T>::equals(*a.begin(), *b.begin())
            && TypeTraits<T>::equals(*a.end(), *b.end());
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<typename T>
struct TypeTraits<RangeInclusive<T>> final : public Details::TypeTraits<RangeInclusive<T>> {
    static constexpr auto equals(Range<T> const& a, Range<T> const& b) -> bool {
        return TypeTraits<T>::equals(*a.begin(), *b.begin())
            && TypeTraits<T>::equals(*a.end(), *b.end());
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};
