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
#include <LibGraphics/Metrics/Point.hh>

namespace Graphics::Metrics {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief Insets descriptor
 */
class Insets {
public:
    /**
     * @brief Constructors
     */
    Insets()              = default;
    Insets(const Insets&) = default;
    Insets(i32 top, i32 left, i32 bottom, i32 right)
        : m_top{ top }
        , m_left{ left }
        , m_bottom{ bottom }
        , m_right{ right } {}

    /**
     * @brief Assignment operators
     */
    Insets& operator=(const Insets&) = default;

    /**
     * @brief Comparison operators
     */
    bool operator==(const Insets& rhs) const {
        return m_top == rhs.m_top && m_left == rhs.m_left && m_bottom == rhs.m_bottom && m_right == rhs.m_right;
    }
    bool operator!=(const Insets& rhs) const { return !(*this == rhs); }

    /**
     * @brief Get/Set for m_top
     */
    [[nodiscard]] i32 top() const { return m_top; }
    void              set_top(i32 top) { m_top = top; }

    /**
     * @brief Get/Set for m_left
     */
    [[nodiscard]] i32 left() const { return m_left; }
    void              set_left(i32 left) { m_left = left; }

    /**
     * @brief Get/Set for m_bottom
     */
    [[nodiscard]] i32 bottom() const { return m_bottom; }
    void              set_bottom(i32 bottom) { m_bottom = bottom; }

    /**
     * @brief Get/Set for m_right
     */
    [[nodiscard]] i32 right() const { return m_right; }
    void              set_right(i32 right) { m_right = right; }

private:
    i32 m_top{ 0 };
    i32 m_left{ 0 };
    i32 m_bottom{ 0 };
    i32 m_right{ 0 };
} A_PACKED;

} /* namespace Graphics::Metrics */