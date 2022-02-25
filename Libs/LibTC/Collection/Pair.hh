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

#include <LibTC/Cxx.hh>

namespace TC {
namespace Collection {

template<typename K, typename T>
class Pair {
public:
    /**
     * @brief Constructors
     */
    Pair() = default;
    Pair(K const& key, T const& value)
        : m_key{ key }
        , m_value{ value } {
    }
    Pair(K&& key, T&& value)
        : m_key{ move(key) }
        , m_value{ move(value) } {
    }
    Pair(Pair const&) = default;
    Pair(Pair&& rhs) noexcept
        : m_key{ move(rhs.m_key) }
        , m_value{ move(rhs.m_value) } {
    }

    ~Pair() = default;

    Pair& operator=(Pair const&) = default;
    Pair& operator=(Pair&&) noexcept = default;

    /**
     * @brief Key getters
     */
    K& key() {
        return m_key;
    }
    K const& key() const {
        return m_key;
    }

    /**
     * @brief Value getters
     */
    T& value() {
        return m_value;
    }
    T const& value() const {
        return m_value;
    }

private:
    K m_key{};
    T m_value{};
};

} /* namespace Collection */

using Collection::Pair;

} /* namespace TC */
