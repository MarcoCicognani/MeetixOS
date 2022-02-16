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
    Pair(Pair const&) = default;
    Pair(Pair&& rhs) noexcept;

    ~Pair() = default;

    Pair& operator=(Pair const&) = default;
    Pair& operator=(Pair&&) noexcept = default;

    /**
     * @brief Key getters
     */
    K&       key();
    K const& key() const;

    /**
     * @brief Value getters
     */
    T&       value();
    T const& value() const;

private:
    K m_key{};
    T m_value{};
};

} /* namespace Collection */

using Collection::Pair;

} /* namespace TC */

#include <LibTC/Collection/Pair.hhi>