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
#include <Graphics/Cairo.hh>
#include <Graphics/Metrics/Dimension.hh>
#include <Graphics/Metrics/Point.hh>
#include <Graphics/Metrics/Rectangle.hh>
#include <string>

namespace Graphics {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief Graphic context which holds a drawable surface
 */
class Context {
public:
    /**
     * @brief Constructor
     */
    Context(int width = 0, int height = 0);

    /**
     * @brief Resizes the context's surface to the given dimension
     */
    void resize(int width, int height);

    /**
     * @brief Blit this context into the given context
     */
    void blit_to(Context*                  other_ctx,
                 const Metrics::Rectangle& absolute_clip,
                 const Metrics::Point&     position);

    /**
     * @brief Returns the inner cairo-context
     */
    [[nodiscard]] cairo_t* cairo_context() const {
        return m_context;
    }

    /**
     * @brief Returns the inner cairo-surface
     */
    [[nodiscard]] cairo_surface_t* cairo_surface() const {
        return m_surface;
    }

    /**
     * @brief Returns the width of the context's surface
     */
    [[nodiscard]] int width() const {
        return m_width;
    }

    /**
     * @brief Returns the height of the context's surface
     */
    [[nodiscard]] int height() const {
        return m_height;
    }

private:
    int              m_width{ 0 };
    int              m_height{ 0 };
    cairo_t*         m_context{ nullptr };
    cairo_surface_t* m_surface{ nullptr };
};

} /* namespace Graphics */