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
 * @brief Graphical rectangle
 */
struct Rectangle {
public:
    Rectangle()                 = default;
    Rectangle(const Rectangle&) = default;
    Rectangle(i32 x, i32 y, i32 width, i32 height)
        : m_x{ x }
        , m_y{ y }
        , m_width{ width }
        , m_height{ height } {}

    /**
     * @brief Assignment operators
     */
    Rectangle& operator=(const Rectangle&) = default;

    /**
     * return true if p is contained into rectangle area
     */
    [[nodiscard]] bool contains(const Point& p) const {
        return (p.x() >= m_x) && (p.y() >= m_y) && (p.x() < m_x + m_width) && (p.y() < m_y + m_height);
    }

    /**
     * @brief Comparison operators
     */
    bool operator==(const Rectangle& rhs) const {
        return m_x == rhs.m_x && m_y == rhs.m_y && m_width == rhs.m_width && m_height == rhs.m_height;
    }
    bool operator!=(const Rectangle& rhs) const { return !(*this == rhs); }

    /**
     * @brief Get/Set for m_y
     */
    [[nodiscard]] i32 y() const { return m_y; }
    [[nodiscard]] i32 top() const { return m_y; }
    void              set_top(i32 top) { m_y = top; }

    /**
     * @brief Get/Set for m_y
     */
    [[nodiscard]] i32 x() const { return m_x; }
    [[nodiscard]] i32 left() const { return m_x; }
    void              set_left(i32 left) { m_x = left; }

    /**
     * @brief Get/Set for m_height
     */
    [[nodiscard]] i32 height() const { return m_height; }
    void              set_height(i32 height) { m_height = height; }

    /**
     * @brief Get/Set for bottom-point
     */
    [[nodiscard]] i32 bottom() const { return m_y + m_height; }
    void              set_bottom(i32 bottom) { m_height = bottom - m_y; }

    /**
     * @brief Get/Set for m_width
     */
    [[nodiscard]] i32 width() const { return m_width; }
    void              set_width(i32 width) { m_width = width; }

    /**
     * @brief Get/Set for right-point
     */
    [[nodiscard]] i32 right() const { return m_x + m_width; }
    void              set_right(i32 right) { m_width = right - m_x; }

    /**
     * @brief Get/Set for start-point
     */
    [[nodiscard]] Point start() const { return { m_x, m_y }; }
    void                set_start(const Point& p) {
        m_x = p.x();
        m_y = p.y();
    }

    /**
     * @brief Get/Set for end-point
     */
    [[nodiscard]] Point end() const { return { m_x + m_width, m_y + m_height }; }
    void                set_end(const Point& p) {
        m_width  = p.x() - m_x;
        m_height = p.y() - m_y;
    }

    /**
     * @brief Normalizes the dimension of this rectangle
     */
    Rectangle as_normalized() {
        auto rect = *this;
        if ( rect.m_width < 0 ) {
            rect.m_x     = rect.m_x + rect.m_width;
            rect.m_width = -rect.m_width;
        }
        if ( rect.m_height < 0 ) {
            rect.m_y      = rect.m_y + rect.m_height;
            rect.m_height = -rect.m_height;
        }
        return rect;
    }

private:
    i32 m_x{ 0 };
    i32 m_y{ 0 };
    i32 m_width{ 0 };
    i32 m_height{ 0 };
} A_PACKED;

} /* namespace Graphics::Metrics */