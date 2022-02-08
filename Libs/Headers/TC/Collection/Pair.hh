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

#include <TC/Cxx/Move.hh>

namespace TC {
namespace Collection {

template<typename K, typename T>
class Pair {
public:
    /**
     * @brief Constructors
     */
    Pair() = default;
    Pair(K const& key, T const& value);
    Pair(K&& key, T&& value);
    Pair(Pair const&)     = default;
    Pair(Pair&&) noexcept = default;

    ~Pair() = default;

    Pair& operator=(Pair const&) = default;
    Pair& operator=(Pair&&) noexcept = default;

    K&       key();
    K const& key() const;

    T&       value();
    T const& value() const;

private:
    K m_key{};
    T m_value{};
};

template<typename K, typename T>
Pair<K, T>::Pair(K const& key, T const& value)
    : m_key{ key }
    , m_value{ value } {
}

template<typename K, typename T>
Pair<K, T>::Pair(K&& key, T&& value)
    : m_key{ move(key) }
    , m_value{ move(value) } {
}

template<typename K, typename T>
K& Pair<K, T>::key() {
    return m_key;
}

template<typename K, typename T>
K const& Pair<K, T>::key() const {
    return m_key;
}

template<typename K, typename T>
T& Pair<K, T>::value() {
    return m_value;
}

template<typename K, typename T>
T const& Pair<K, T>::value() const {
    return m_value;
}

} /* namespace Collection */

using Collection::Pair;

} /* namespace TC */