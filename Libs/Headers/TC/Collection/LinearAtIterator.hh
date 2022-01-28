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

namespace TC::Collection {

template<class Container, typename T>
class LinearAtIterator {
public:
    /**
     * @brief Construction functions
     */
    static LinearAtIterator begin(Container& container);
    static LinearAtIterator end(Container& container);

    LinearAtIterator(LinearAtIterator const&) = default;

    LinearAtIterator& operator=(LinearAtIterator const&) = default;

    /**
     * @brief Increment operators
     */
    LinearAtIterator& operator++();
    LinearAtIterator  operator++(int);

    /**
     * @brief ValueReference access operators
     */
    T&       operator*();
    T const& operator*() const;

    /**
     * @brief Pointer access operators
     */
    T*       operator->();
    T const* operator->() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] bool  is_end() const;
    [[nodiscard]] usize index() const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(LinearAtIterator const& rhs) const;
    [[nodiscard]] bool operator!=(LinearAtIterator const& rhs) const;
    [[nodiscard]] bool operator<(LinearAtIterator const& rhs) const;
    [[nodiscard]] bool operator>(LinearAtIterator const& rhs) const;
    [[nodiscard]] bool operator<=(LinearAtIterator const& rhs) const;
    [[nodiscard]] bool operator>=(LinearAtIterator const& rhs) const;

    /**
     * @brief Math operators
     */
    [[nodiscard]] LinearAtIterator operator+(ptrdiff_t delta) const;
    [[nodiscard]] LinearAtIterator operator-(ptrdiff_t delta) const;

    [[nodiscard]] ptrdiff_t operator-(LinearAtIterator rhs) const;

private:
    LinearAtIterator(Container& container, usize index);

private:
    Container* m_container{ nullptr };
    usize      m_index{ 0 };
};

template<class Container, typename T>
LinearAtIterator<Container, T> LinearAtIterator<Container, T>::begin(Container& container) {
    return LinearAtIterator{ container, 0 };
}

template<class Container, typename T>
LinearAtIterator<Container, T> LinearAtIterator<Container, T>::end(Container& container) {
    return LinearAtIterator{ container, container.count() };
}

template<class Container, typename T>
LinearAtIterator<Container, T>& LinearAtIterator<Container, T>::operator++() {
    ++m_index;
    return *this;
}

template<class Container, typename T>
LinearAtIterator<Container, T> LinearAtIterator<Container, T>::operator++(int) {
    ++m_index;
    return LinearAtIterator{ m_container, m_index - 1 };
}

template<class Container, typename T>
T& LinearAtIterator<Container, T>::operator*() {
    return m_container->at(m_index);
}

template<class Container, typename T>
T const& LinearAtIterator<Container, T>::operator*() const {
    return m_container->at(m_index);
}

template<class Container, typename T>
T* LinearAtIterator<Container, T>::operator->() {
    return &operator*();
}

template<class Container, typename T>
T const* LinearAtIterator<Container, T>::operator->() const {
    return &operator*();
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::is_end() const {
    return m_index == end(*m_container).index();
}

template<class Container, typename T>
usize LinearAtIterator<Container, T>::index() const {
    return m_index;
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::operator==(LinearAtIterator const& rhs) const {
    return m_container == rhs.m_container && m_index == rhs.m_index;
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::operator!=(LinearAtIterator const& rhs) const {
    return m_container != rhs.m_container || m_index != rhs.m_index;
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::operator<(LinearAtIterator const& rhs) const {
    return m_index < rhs.m_index;
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::operator>(LinearAtIterator const& rhs) const {
    return m_index > rhs.m_index;
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::operator<=(LinearAtIterator const& rhs) const {
    return m_index <= rhs.m_index;
}

template<class Container, typename T>
bool LinearAtIterator<Container, T>::operator>=(LinearAtIterator const& rhs) const {
    return m_index >= rhs.m_index;
}

template<class Container, typename T>
LinearAtIterator<Container, T> LinearAtIterator<Container, T>::operator+(ptrdiff_t delta) const {
    return LinearAtIterator{ m_container, m_index + delta };
}

template<class Container, typename T>
LinearAtIterator<Container, T> LinearAtIterator<Container, T>::operator-(ptrdiff_t delta) const {
    return LinearAtIterator{ m_container, m_index - delta };
}

template<class Container, typename T>
ptrdiff_t LinearAtIterator<Container, T>::operator-(LinearAtIterator rhs) const {
    return static_cast<ptrdiff_t>(m_index) - rhs.m_index;
}

template<class Container, typename T>
LinearAtIterator<Container, T>::LinearAtIterator(Container& container, usize index)
    : m_container{ &container }
    , m_index{ index } {
}

} /* namespace TC::Collection */