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
 * container class to manage point on screen
 */
class Point {
public:
    /**
     * @brief Constructors
     */
    Point()             = default;
    Point(const Point&) = default;
    Point(i32 x, i32 y)
        : m_x{ x }
        , m_y{ y } {}

    /**
     * @brief Assignment operators
     */
    Point& operator=(const Point&) = default;

    /**
     * @brief Comparison operators
     */
    bool operator==(const Point& p) const { return m_x == p.m_x && m_y == p.m_y; }
    bool operator!=(const Point& p) const { return !(*this == p); }

    /**
     * @brief Arithmetic operators
     */
    Point operator-(const Point& p) const { return { m_x - p.m_x, m_y - p.m_y }; }
    Point operator+(const Point& p) const { return { m_x + p.m_x, m_y + p.m_y }; }

    /**
     * @brief Get/Set for m_x
     */
    [[nodiscard]] i32 x() const { return m_x; }
    void              set_x(i32 x) { m_x = x; }

    /**
     * @brief Get/Set for m_y
     */
    [[nodiscard]] i32 y() const { return m_y; }
    void              set_y(i32 y) { m_y = y; }

private:
    i32 m_x{ 0 };
    i32 m_y{ 0 };
} A_PACKED;

} /* namespace Graphics::Metrics */