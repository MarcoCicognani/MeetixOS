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

#include <cstdint>

namespace Graphics::Metrics {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief Graphical ranges descriptor
 */
class Range {
public:
    /**
     * @brief Constructors
     */
    Range()             = default;
    Range(const Range&) = default;
    Range(i32 a, i32 b) { set(a, b); }

    /**
     * @brief Assignment operators
     */
    Range& operator=(const Range&) = default;

    /**
     * @brief Sets the range with the given values
     */
    void set(i32 a, i32 b) {
        m_first = std::min(a, b);
        m_last  = std::max(a, b);
    }

    /**
     * @brief Comparison operators
     */
    bool operator==(const Range& p) const { return (m_first == p.m_first) && (m_last == p.m_last); }
    bool operator!=(const Range& p) const { return !(*this == p); }

    /**
     * @brief Returns the begin of the range
     */
    [[nodiscard]] i32 first() const { return m_first; }

    /**
     * @brief Returns the end of the range
     */
    [[nodiscard]] i32 last() const { return m_last; }

    /**
     * @brief Returns the range's len
     */
    [[nodiscard]] i32 len() const { return m_last - m_first; }

private:
    i32 m_first{ 0 };
    i32 m_last{ 0 };
} A_PACKED;

} /* namespace Graphics::Metrics */