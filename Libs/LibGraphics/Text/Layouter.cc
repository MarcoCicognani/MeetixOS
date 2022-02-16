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

#include <Graphics/Text/Layouter.hh>
#include <LibUtils/Utils.hh>

namespace Graphics::Text {

Layouter& Layouter::instance() {
    static Layouter* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new Layouter();
    return *s_instance;
}

Layouted* Layouter::init_buffer() {
    return new Layouted();
}

void Layouter::layout(cairo_t*                     cr,
                      const char*                  text,
                      Font*                        font,
                      int32_t                      size,
                      Graphics::Metrics::Rectangle bounds,
                      const Alignment&             alignment,
                      Layouted*                    layout,
                      bool                         break_on_overflow) {
    /* check font validity */
    if ( !font )
        return;

    /* calculate the text-length */
    auto text_len = strlen(text);

    /* starting coordinates */
    auto x            = bounds.x();
    auto y            = bounds.y();
    auto line_start_x = x;

    /* created the scaled font face */
    cairo_set_font_face(cr, font->cairo_font_face());
    cairo_set_font_size(cr, size);
    auto cairo_scaled_face = cairo_get_scaled_font(cr);

    auto line        = 0;
    auto line_height = size;

    /* create glyphs for the text */
    auto previous_glyph_buffer   = layout->m_cairo_glyph;
    auto previous_cluster_buffer = layout->m_cairo_text_cluster;

    cairo_text_cluster_flags_t cairo_text_cluster_flags;
    auto                       cairo_status = cairo_scaled_font_text_to_glyphs(cairo_scaled_face,
                                                         0,
                                                         0,
                                                         text,
                                                         static_cast<int>(text_len),
                                                         &layout->m_cairo_glyph,
                                                         &layout->m_glyph_count,
                                                         &layout->m_cairo_text_cluster,
                                                         &layout->m_cluster_count,
                                                         &cairo_text_cluster_flags);

    /* destroy previous buffer */
    if ( previous_glyph_buffer && layout->m_cairo_glyph != previous_glyph_buffer )
        free(previous_glyph_buffer);
    if ( previous_cluster_buffer && layout->m_cairo_text_cluster != previous_cluster_buffer )
        free(previous_cluster_buffer);

    /* clear layout entries */
    layout->m_positioned_glyphs.clear();

    /* perform layouting */
    if ( cairo_status == CAIRO_STATUS_SUCCESS ) {
        /* positions in bytes and glyphs */
        auto byte_pos  = 0;
        auto glyph_pos = 0;

        /* text extents */
        cairo_text_extents_t extents;
        for ( auto i = 0; i < layout->m_cluster_count; ++i ) {
            auto cluster = &layout->m_cairo_text_cluster[i];
            auto glyphs  = &layout->m_cairo_glyph[glyph_pos];

            /* create new position */
            PositionedGlyph positioned_glyph;
            positioned_glyph.m_cairo_glyph       = glyphs;
            positioned_glyph.m_cairo_glyph_count = cluster->num_glyphs;
            cairo_scaled_font_glyph_extents(cairo_scaled_face,
                                            positioned_glyph.m_cairo_glyph,
                                            positioned_glyph.m_cairo_glyph_count,
                                            &extents);

            positioned_glyph.m_advance.set_x(static_cast<i32>(extents.x_advance));
            positioned_glyph.m_advance.set_y(static_cast<i32>(extents.y_advance));
            positioned_glyph.m_size.set_width(static_cast<i32>(extents.width));
            positioned_glyph.m_size.set_height(static_cast<i32>(extents.height));

            /* check if newline */
            bool is_newline = false;
            bool invisible  = false;
            if ( cluster->num_bytes == 1 && text[byte_pos] == '\n' )
                is_newline = true;

            /* wouldn't match in line or is break character? Start next line */
            if ( is_newline || (break_on_overflow && (x + positioned_glyph.m_size.width() > bounds.width())) ) {
                if ( is_newline )
                    invisible = true;
                if ( alignment == Alignment::RIGHT )
                    right_align(layout, line, x - line_start_x, bounds);
                else if ( alignment == Alignment::CENTER )
                    center_align(layout, line, x - line_start_x, bounds);

                ++line;
                x            = bounds.x();
                line_start_x = x;
                y += line_height;
            }

            if ( !invisible ) {
                /* position the glyph */
                positioned_glyph.m_line = line;
                positioned_glyph.m_position.set_x(x);
                positioned_glyph.m_position.set_y(y + line_height);

                /* add to the list and go to next place */
                layout->m_positioned_glyphs.push_back(positioned_glyph);
                x += positioned_glyph.m_advance.x();
            }

            /* increase positions */
            glyph_pos += cluster->num_glyphs;
            byte_pos += cluster->num_bytes;
        }
    }

    /* perform last alignment */
    if ( alignment == Alignment::RIGHT )
        right_align(layout, line, x - line_start_x, bounds);
    else if ( alignment == Alignment::CENTER )
        center_align(layout, line, x - line_start_x, bounds);

#define BOUNDS_EMPTY 0xFFFFFF

    /* set text bounds */
    auto text_bound_top    = BOUNDS_EMPTY;
    auto text_bound_left   = BOUNDS_EMPTY;
    auto text_bound_right  = 0;
    auto text_bound_bottom = 0;
    for ( auto& p_glyph : layout->m_positioned_glyphs ) {
        if ( p_glyph.m_position.x() < text_bound_left )
            text_bound_left = p_glyph.m_position.x();
        if ( p_glyph.m_position.y() < text_bound_top )
            text_bound_top = p_glyph.m_position.y();

        /* get extents again */
        auto right = p_glyph.m_position.x() + p_glyph.m_size.width();
        if ( right > text_bound_right )
            text_bound_right = right;

        auto bottom = p_glyph.m_position.y() + p_glyph.m_size.height();
        if ( bottom > text_bound_bottom )
            text_bound_bottom = bottom;
    }

    /* set the all-text bounds */
    if ( text_bound_top != BOUNDS_EMPTY && text_bound_left != BOUNDS_EMPTY ) {
        layout->m_text_bounds.set_left(text_bound_left);
        layout->m_text_bounds.set_top(text_bound_top);
        layout->m_text_bounds.set_width(text_bound_right - text_bound_left);
        layout->m_text_bounds.set_height(text_bound_bottom - text_bound_top);
    }
}

void Layouter::destroy_layout(Layouted* layouted_text) {
    if ( layouted_text->m_cairo_glyph )
        free(layouted_text->m_cairo_glyph);
    if ( layouted_text->m_cairo_text_cluster )
        free(layouted_text->m_cairo_text_cluster);
    delete layouted_text;
}

void Layouter::right_align(Layouted* text, usize line, usize width, Graphics::Metrics::Rectangle& bounds) {
    auto difference = bounds.width() - bounds.x() - width;
    for ( auto& p_glyph : text->m_positioned_glyphs ) {
        if ( p_glyph.m_line == line )
            p_glyph.m_position.set_x(static_cast<i32>(p_glyph.m_position.x() + difference));
    }
}

void Layouter::center_align(Layouted* text, usize line, usize width, Graphics::Metrics::Rectangle& bounds) {
    auto difference = (bounds.width() - bounds.x()) / 2 - width / 2;
    for ( auto& p_glyph : text->m_positioned_glyphs ) {
        if ( p_glyph.m_line == line )
            p_glyph.m_position.set_x(static_cast<i32>(p_glyph.m_position.x() + difference));
    }
}

} /* namespace Graphics::Text */
