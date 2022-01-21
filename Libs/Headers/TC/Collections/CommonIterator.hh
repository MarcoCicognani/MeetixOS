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

#include <TC/Functional/Option.hh>
#include <TC/IntTypes.hh>

namespace TC::Collections {

template<class Container, typename T>
class CommonIterator {
public:
    /**
     * @brief Construction functions
     */
    static CommonIterator begin(Container& container);
    static CommonIterator end(Container& container);

    /**
     * @brief Increment operators
     */
    CommonIterator& operator++();
    CommonIterator  operator++(int);

    /**
     * @brief Decrement operators
     */
    CommonIterator& operator--();
    CommonIterator  operator--(int);

    /**
     * @brief Reference access operators
     */
    T const& operator*() const;
    T&       operator*();

    /**
     * @brief Pointer access operators
     */
    T const* operator->() const;
    T*       operator->();

    /**
     * @brief Getters
     */
    [[nodiscard]] bool  is_end() const;
    [[nodiscard]] usize index() const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(CommonIterator rhs) const;
    [[nodiscard]] bool operator!=(CommonIterator rhs) const;
    [[nodiscard]] bool operator<(CommonIterator rhs) const;
    [[nodiscard]] bool operator>(CommonIterator rhs) const;
    [[nodiscard]] bool operator<=(CommonIterator rhs) const;
    [[nodiscard]] bool operator>=(CommonIterator rhs) const;

    /**
     * @brief Math operators
     */
    [[nodiscard]] CommonIterator operator+(ptrdiff_t delta) const;
    [[nodiscard]] CommonIterator operator-(ptrdiff_t delta) const;

    [[nodiscard]] ptrdiff_t operator-(CommonIterator rhs) const;

private:
    CommonIterator(Container& container, usize index);

private:
    Container& m_container;
    usize      m_index{ 0 };
};

template<class Container, typename T>
CommonIterator<Container, T> CommonIterator<Container, T>::begin(Container& container) {
    return CommonIterator{ container, 0 };
}

template<class Container, typename T>
CommonIterator<Container, T> CommonIterator<Container, T>::end(Container& container) {
    return CommonIterator{ container, container.count() };
}

template<class Container, typename T>
CommonIterator<Container, T>& CommonIterator<Container, T>::operator++() {
    ++m_index;
    return *this;
}

template<class Container, typename T>
CommonIterator<Container, T> CommonIterator<Container, T>::operator++(int) {
    ++m_index;
    return CommonIterator{ m_container, m_index - 1 };
}

template<class Container, typename T>
CommonIterator<Container, T>& CommonIterator<Container, T>::operator--() {
    --m_index;
    return *this;
}

template<class Container, typename T>
CommonIterator<Container, T> CommonIterator<Container, T>::operator--(int) {
    --m_index;
    return CommonIterator{ m_container, m_index + 1 };
}

template<class Container, typename T>
T const& CommonIterator<Container, T>::operator*() const {
    return m_container[m_index];
}

template<class Container, typename T>
T& CommonIterator<Container, T>::operator*() {
    return m_container[m_index];
}

template<class Container, typename T>
T const* CommonIterator<Container, T>::operator->() const {
    return &operator*();
}

template<class Container, typename T>
T* CommonIterator<Container, T>::operator->() {
    return &operator*();
}

template<class Container, typename T>
bool CommonIterator<Container, T>::is_end() const {
    return m_index == end(m_container).index();
}

template<class Container, typename T>
usize CommonIterator<Container, T>::index() const {
    return m_index;
}

template<class Container, typename T>
bool CommonIterator<Container, T>::operator==(CommonIterator rhs) const {
    return m_index == rhs.m_index;
}

template<class Container, typename T>
bool CommonIterator<Container, T>::operator!=(CommonIterator rhs) const {
    return m_index != rhs.m_index;
}

template<class Container, typename T>
bool CommonIterator<Container, T>::operator<(CommonIterator rhs) const {
    return m_index < rhs.m_index;
}

template<class Container, typename T>
bool CommonIterator<Container, T>::operator>(CommonIterator rhs) const {
    return m_index > rhs.m_index;
}

template<class Container, typename T>
bool CommonIterator<Container, T>::operator<=(CommonIterator rhs) const {
    return m_index <= rhs.m_index;
}

template<class Container, typename T>
bool CommonIterator<Container, T>::operator>=(CommonIterator rhs) const {
    return m_index >= rhs.m_index;
}

template<class Container, typename T>
CommonIterator<Container, T> CommonIterator<Container, T>::operator+(ptrdiff_t delta) const {
    return CommonIterator{ m_container, m_index + delta };
}

template<class Container, typename T>
CommonIterator<Container, T> CommonIterator<Container, T>::operator-(ptrdiff_t delta) const {
    return CommonIterator{ m_container, m_index - delta };
}

template<class Container, typename T>
ptrdiff_t CommonIterator<Container, T>::operator-(CommonIterator rhs) const {
    return static_cast<ptrdiff_t>(m_index) - rhs.m_index;
}

template<class Container, typename T>
CommonIterator<Container, T>::CommonIterator(Container& container, usize index)
    : m_container{ container }
    , m_index{ index } {
}

} /* namespace TC::Collections */