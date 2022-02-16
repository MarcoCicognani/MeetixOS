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

#include <LibGraphics/Context.hh>

namespace Graphics {

Context::Context(u32 width, u32 height)
    : m_width{ width }
    , m_height{ height } {
    resize(width, height);
}

void Context::resize(u32 width, u32 height) {
    if ( !width || !height )
        return;

    /* release the old surfaces */
    if ( m_surface )
        cairo_surface_destroy(m_surface);
    if ( m_context )
        cairo_destroy(m_context);

    /* set the new values and re-create the surface */
    m_width   = width;
    m_height  = height;
    m_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, static_cast<i32>(width), static_cast<i32>(height));
    m_context = cairo_create(m_surface);
}

void Context::blit_to(Context* other_ctx, const Metrics::Rectangle& absolute_clip, const Metrics::Point& position) {
    auto cr = other_ctx->cairo_context();

    cairo_save(cr);
    cairo_set_source_surface(cr, this->m_surface, position.x(), position.y());
    cairo_rectangle(cr, absolute_clip.x(), absolute_clip.y(), absolute_clip.width(), absolute_clip.height());
    cairo_clip(cr);
    cairo_paint(cr);
    cairo_restore(cr);
}

} /* namespace Graphics */