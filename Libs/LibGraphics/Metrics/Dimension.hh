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

#include <cstdint>
#include <LibGraphics/Metrics/Point.hh>

namespace Graphics::Metrics {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief Dimension descriptor
 */
class Dimension {
public:
    /**
     * @brief Constructors
     */
    Dimension()                 = default;
    Dimension(const Dimension&) = default;
    Dimension(i32 width, i32 height)
        : m_width{ width }
        , m_height{ height } {}

    /**
     * @brief Assignment operators
     */
    Dimension& operator=(const Dimension&) = default;

    /**
     * @brief Comparison operators
     */
    bool operator==(const Dimension& rhs) const { return m_width == rhs.m_width && m_height == rhs.m_height; }
    bool operator!=(const Dimension& p) const { return !(*this == p); }

    /**
     * @brief Arithmetic operators
     */
    Point operator-(const Dimension& rhs) const { return { m_width - rhs.m_width, m_height - rhs.m_height }; }
    Point operator+(const Dimension& rhs) const { return { m_width + rhs.m_width, m_height + rhs.m_height }; }

    /**
     * @brief Get/Set for m_width
     */
    [[nodiscard]] i32 width() const { return m_width; }
    void              set_width(i32 width) { m_width = width; }

    /**
     * @brief Get/Set for m_height
     */
    [[nodiscard]] i32 height() const { return m_height; }
    void              set_height(i32 height) { m_height = height; }

private:
    i32 m_width{ 0 };
    i32 m_height{ 0 };
} A_PACKED;

} /* namespace Graphics::Metrics */