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

namespace TC::Collection {

template<typename MainContainer, typename InnerContainer, typename T>
class NestedIterator {
public:
    /**
     * @brief Construction functions
     */
    static NestedIterator begin(MainContainer& main_container);
    static NestedIterator end(MainContainer& main_container);

    NestedIterator(NestedIterator const&) = default;

    NestedIterator& operator=(NestedIterator const&) = default;

    /**
     * @brief Increment operators
     */
    NestedIterator& operator++();
    NestedIterator  operator++(int);

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
    [[nodiscard]] bool is_end() const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(NestedIterator const& rhs) const;
    [[nodiscard]] bool operator!=(NestedIterator const& rhs) const;
    [[nodiscard]] bool operator<=>(NestedIterator const&) const = default;

private:
    NestedIterator(MainContainer&                    main_container,
                   usize                             first_level_index,
                   typename InnerContainer::Iterator nested_iterator);

private:
    MainContainer&                    m_main_container;
    usize                             m_first_level_index{ 0 };
    typename InnerContainer::Iterator m_nested_iterator{};
};

template<typename MainContainer, typename InnerContainer, typename T>
NestedIterator<MainContainer, InnerContainer, T>::NestedIterator(MainContainer&                    main_container,
                                                                 usize                             first_level_index,
                                                                 typename InnerContainer::Iterator nested_iterator)
    : m_main_container{ main_container }
    , m_first_level_index{ first_level_index }
    , m_nested_iterator{ nested_iterator } {
}

template<typename MainContainer, typename InnerContainer, typename T>
NestedIterator<MainContainer, InnerContainer, T>
NestedIterator<MainContainer, InnerContainer, T>::begin(MainContainer& main_container) {
    usize first_level_index = 0;
    for ( auto const& inner : main_container.data() ) {
        if ( !inner.is_empty() )
            break;

        ++first_level_index;
    }

    return NestedIterator{ main_container, first_level_index, main_container.data().at(first_level_index).begin() };
}

template<typename MainContainer, typename InnerContainer, typename T>
NestedIterator<MainContainer, InnerContainer, T>
NestedIterator<MainContainer, InnerContainer, T>::end(MainContainer& main_container) {
    return NestedIterator{ main_container, main_container.data().count() - 1, main_container.data().last().end() };
}

template<typename MainContainer, typename InnerContainer, typename T>
NestedIterator<MainContainer, InnerContainer, T>& NestedIterator<MainContainer, InnerContainer, T>::operator++() {
    ++m_nested_iterator;
    if ( m_nested_iterator.is_end() ) {
        /* find the next non-empty inner collector */
        while ( ++m_first_level_index < m_main_container.data().count() ) {
            auto& inner = m_main_container.data().at(m_first_level_index);

            /* always assign the iterator even the <inner> is empty, since the operator!= and == of the linear
             * iterator checks whether the source container is the same, and our end container is the last of
             * the m_main_container.data() */
            m_nested_iterator = inner.begin();
            if ( !inner.is_empty() )
                break;
        }
    }

    return *this;
}

template<typename MainContainer, typename InnerContainer, typename T>
NestedIterator<MainContainer, InnerContainer, T> NestedIterator<MainContainer, InnerContainer, T>::operator++(int) {
    NestedIterator it{ *this };

    this->operator++();
    return it;
}

template<typename MainContainer, typename InnerContainer, typename T>
T& NestedIterator<MainContainer, InnerContainer, T>::operator*() {
    return *m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
T const& NestedIterator<MainContainer, InnerContainer, T>::operator*() const {
    return *m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
T* NestedIterator<MainContainer, InnerContainer, T>::operator->() {
    return &operator*();
}

template<typename MainContainer, typename InnerContainer, typename T>
T const* NestedIterator<MainContainer, InnerContainer, T>::operator->() const {
    return &operator*();
}

template<typename MainContainer, typename InnerContainer, typename T>
bool NestedIterator<MainContainer, InnerContainer, T>::is_end() const {
    return m_nested_iterator.is_end();
}

template<typename MainContainer, typename InnerContainer, typename T>
bool NestedIterator<MainContainer, InnerContainer, T>::operator==(const NestedIterator& rhs) const {
    return m_nested_iterator == rhs.m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
bool NestedIterator<MainContainer, InnerContainer, T>::operator!=(const NestedIterator& rhs) const {
    return m_nested_iterator != rhs.m_nested_iterator;
}

} /* namespace TC::Collection */