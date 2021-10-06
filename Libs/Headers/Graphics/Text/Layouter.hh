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

#include <Graphics/Metrics/Dimension.hh>
#include <Graphics/Metrics/Point.hh>
#include <Graphics/Metrics/Rectangle.hh>
#include <Graphics/Text/Alignment.hh>
#include <Graphics/Text/Font.hh>
#include <vector>

namespace Graphics::Text {

/**
 * @brief Rendered glyph on the screen
 */
struct PositionedGlyph {
public:
    i32                          m_line{ -1 };
    Graphics::Metrics::Point     m_position{};
    Graphics::Metrics::Dimension m_size{};
    Graphics::Metrics::Point     m_advance{};
    cairo_glyph_t*               m_cairo_glyph{ nullptr };
    i32                          m_cairo_glyph_count{ 0 };

    /**
     * @brief Constructors
     */
    PositionedGlyph() = default;
    PositionedGlyph(i32                                 line,
                    const Graphics::Metrics::Point&     position,
                    const Graphics::Metrics::Dimension& size,
                    const Graphics::Metrics::Point&     advance,
                    cairo_glyph_t*                      cairo_glyph,
                    i32                                 cairo_glyph_count)
        : m_line{ line }, m_position{ position }, m_size{ size }, m_advance{ advance },
          m_cairo_glyph{ cairo_glyph }, m_cairo_glyph_count{ cairo_glyph_count } {
    }
};

/**
 * @brief Rendered list of glyphs on the screen
 */
struct Layouted {
public:
    Graphics::Metrics::Rectangle m_text_bounds{};
    cairo_glyph_t*               m_cairo_glyph{ nullptr };
    i32                          m_glyph_count{ 0 };
    cairo_text_cluster_t*        m_cairo_text_cluster{ nullptr };
    i32                          m_cluster_count{ 0 };
    std::vector<PositionedGlyph> m_positioned_glyphs{};

    /**
     * @brief Constructors
     */
    Layouted() = default;
    Layouted(const Graphics::Metrics::Rectangle& text_bounds,
                 cairo_glyph_t*                      cairo_glyph,
                 i32                                 glyph_count,
                 cairo_text_cluster_t*               cairo_text_cluster,
                 i32                                 cluster_count)
        : m_text_bounds{ text_bounds }, m_cairo_glyph{ cairo_glyph }, m_glyph_count{ glyph_count },
          m_cairo_text_cluster{ cairo_text_cluster }, m_cluster_count{ cluster_count } {
    }
};

/**
 * @brief Singleton to manage cairo rendered glyphs
 */
class Layouter {
public:
    /**
     * @brief Returns the instance of the font manager singleton
     */
    static Layouter& instance();

    /**
     * @brief Returns a new LayoutText buffer
     */
    Layouted* init_buffer();

    /**
     * @brief Performs the layout of the given text with the requested parameters
     */
    void layout(cairo_t*                     cr,
                const char*                  text,
                Font*                        font,
                i32                          size,
                Graphics::Metrics::Rectangle bounds,
                const Alignment&         alignment,
                Layouted*                layout,
                bool                         break_on_overflow = true);

    /**
     * @brief Destroys the given layouted text
     */
    void destroy_layout(Layouted* layouted_text);

private:
    /**
     * @brief Constructor
     */
    Layouter() = default;

    void right_align(Layouted*                 layouted_text,
                     usize                         line,
                     usize                         width,
                     Graphics::Metrics::Rectangle& bounds);

    void center_align(Layouted*                 layouted_text,
                      usize                         line,
                      usize                         width,
                      Graphics::Metrics::Rectangle& bounds);
};

} /* namespace Graphics::Text */
