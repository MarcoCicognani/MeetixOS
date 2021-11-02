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

#include "HeadlessGUIScreen.hh"

#include <cstring>
#include <Graphics/Color.hh>
#include <Graphics/Text/FontLoader.hh>
#include <Graphics/Vbe.hh>
#include <Tasking/LockGuard.hh>

bool HeadlessGUIScreen::init() {
    auto mode_is_set = Graphics::Vbe::set_mode(1024, 768, 32, m_vbe_mode_info);
    if ( mode_is_set ) {
        /* allocate the raster buffer */
        m_raster_buffer = new RasterCell[width() * height()]{};
        m_back_context.resize(m_vbe_mode_info.m_width, m_vbe_mode_info.m_height);

        /* load the font for the rendering */
        m_font = Graphics::Text::FontLoader::get("consolas");

        /* start the painter thread */
        m_painter_thread       = new PainterThread{ *this };
        auto painter_thread_id = m_painter_thread->start();

        return painter_thread_id != -1;
    } else
        return false;
}

IO::Keyboard::Info HeadlessGUIScreen::read_input() {
    return IO::Keyboard::instance().read();
}

void HeadlessGUIScreen::clean() {
    Tasking::LockGuard lock_guard{ m_raster_buffer_lock };

    for ( auto i = 0; i < width() * height(); ++i )
        m_raster_buffer[i] = RasterCell{};
    repaint();
}

void HeadlessGUIScreen::backspace() {
    Tasking::LockGuard lock_guard{ m_raster_buffer_lock };

    move_cursor_unlocked(m_cursor_position.x() - 1, m_cursor_position.y());
    write_char_unlocked(' ');
    move_cursor_unlocked(m_cursor_position.x() - 1, m_cursor_position.y());
    repaint();
}

void HeadlessGUIScreen::write_char(char c) {
    if ( !char_is_utf8(c) )
        return;

    write_char_unlocked(c);
    repaint();
}

void HeadlessGUIScreen::move_cursor(int x, int y) {
    Tasking::LockGuard lock_guard{ m_raster_buffer_lock };

    move_cursor_unlocked(x, y);
    repaint();
}

int HeadlessGUIScreen::cursor_x() {
    return m_cursor_position.x();
}

int HeadlessGUIScreen::cursor_y() {
    return m_cursor_position.y();
}

int HeadlessGUIScreen::width() {
    return m_vbe_mode_info.m_width / m_font_dimension.width();
}

int HeadlessGUIScreen::height() {
    return m_vbe_mode_info.m_height / m_font_dimension.height();
}

void HeadlessGUIScreen::set_scroll_area_screen() {
    /* TODO */
}

void HeadlessGUIScreen::set_scroll_area(int start, int end) {
    /* TODO */
}

void HeadlessGUIScreen::scroll(int value) {
    /* TODO */
}

void HeadlessGUIScreen::set_cursor_visible(bool visible) {
    /* TODO */
}

[[noreturn]] void HeadlessGUIScreen::paint() {
    while ( true ) {
        auto render_start = s_millis();

        auto cr              = m_back_context.cairo_context();
        auto argb_background = screen_color_to_argb(color_background());

        /* clear the screen buffer */
        cairo_save(cr);
        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(argb_background));
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr);
        cairo_restore(cr);

        /* paint the characters to the graphics buffer */
        {
            Tasking::LockGuard lock_guard{ m_raster_buffer_lock };

            /* draw the cursor */
            auto blink_on = s_millis() - m_last_input_ts < 300;
            if ( blink_on ) {
                cairo_save(cr);
                cairo_set_source_rgba(cr, 1, 1, 1, 1);
                cairo_rectangle(cr,
                                m_cursor_position.x() * m_font_dimension.width(),
                                m_cursor_position.y() * m_font_dimension.height() + 1,
                                2,
                                m_font_dimension.height() + 1);
                cairo_fill(cr);
                cairo_restore(cr);
            }

            /* resize the font to the given size */
            cairo_set_font_face(cr, m_font->cairo_font_face());
            cairo_set_font_size(cr, 14);

            /* draw each character into the graphic buffer */
            auto cairo_scaled_font = cairo_get_scaled_font(cr);
            for ( auto y = 0; y < height(); ++y ) {
                for ( auto x = 0; x < width(); ++x ) {
                    auto raster_cell = m_raster_buffer[y * width() + x];

                    cairo_save(cr);
                    cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(raster_cell.m_background));
                    cairo_rectangle(cr,
                                    x * m_font_dimension.width(),
                                    (y + 1) * m_font_dimension.height(),
                                    m_font_dimension.width(),
                                    m_font_dimension.height());
                    cairo_fill(cr);
                    cairo_restore(cr);

                    if ( !raster_cell )
                        continue;

                    /* obtain the character glyph to draw */
                    auto char_layout = cached_char_layout(cairo_scaled_font, raster_cell.m_char);
                    if ( char_layout ) {
                        cairo_save(cr);
                        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(raster_cell.m_foreground));
                        cairo_translate(cr,
                                        x * m_font_dimension.width(),
                                        (y + 1) * m_font_dimension.height());
                        cairo_glyph_path(cr,
                                         char_layout->m_cairo_glyph,
                                         char_layout->m_text_cluster[0].num_glyphs);
                        cairo_fill(cr);
                        cairo_restore(cr);
                    }
                }
            }
        }

        /* blit to the screen */
        blit_to_screen();

        /* limit blit to video to 60 fps */
        auto render_time = s_millis() - render_start;
        if ( render_time < 1000 / 60 )
            s_sleep(1000 / 60 - render_time);

        /* block this thread until repaint is called */
        m_render_lock.lock();
    }
}

void HeadlessGUIScreen::blit_to_screen() {
    auto video_buffer  = reinterpret_cast<u8*>(m_vbe_mode_info.m_linear_framebuffer);
    auto source_buffer = reinterpret_cast<Graphics::Color::ArgbGradient*>(
        cairo_image_surface_get_data(m_back_context.cairo_surface()));

    for ( auto y = 0; y < m_vbe_mode_info.m_height; ++y ) {
        auto argb_pixel = reinterpret_cast<Graphics::Color::ArgbGradient*>(video_buffer);
        for ( auto x = 0; x < m_vbe_mode_info.m_width; ++x )
            argb_pixel[x] = source_buffer[y * m_vbe_mode_info.m_width + x];

        /* go to next scan-line */
        video_buffer += m_vbe_mode_info.m_bytes_per_scanline;
    }
}

void HeadlessGUIScreen::repaint() {
    m_render_lock.unlock();
}

void HeadlessGUIScreen::move_cursor_unlocked(int x, int y) {
    m_cursor_position = Graphics::Metrics::Point{ x, y };

    /* check for screen bounds */
    if ( m_cursor_position.x() >= width() )
        m_cursor_position = Graphics::Metrics::Point{ 0, y + 1 };
    if ( m_cursor_position.x() < 0 )
        m_cursor_position = Graphics::Metrics::Point{ width() - 1, y - 1 };

    /* scroll screen if required */
    if ( m_cursor_position.y() >= height() ) {
        auto buffer_width = width();
        auto buffer_bytes = buffer_width * height() * sizeof(RasterCell);

        /* move all one line up */
        std::memmove(m_raster_buffer, &m_raster_buffer[buffer_width], buffer_bytes - buffer_width);

        /* clean the last line */
        for ( auto i = 0; i < width(); ++i )
            m_raster_buffer[buffer_bytes - buffer_width + i]
                = RasterCell{ '\0', color_background(), color_foreground() };

        /* back the cursor */
        m_cursor_position.set_y(m_cursor_position.y() - 1);
    }
}

void HeadlessGUIScreen::write_char_unlocked(char c) {
    if ( c == '\n' )
        move_cursor_unlocked(0, m_cursor_position.y() + 1);
    else {
        /* append the new character to the buffer */
        auto raster_buffer_pos = m_cursor_position.y() * width() + m_cursor_position.x();
        m_raster_buffer[raster_buffer_pos]
            = RasterCell{ c, color_background(), color_foreground() };
        m_last_input_ts = s_millis();

        /* advance the cursor */
        move_cursor_unlocked(m_cursor_position.x() + 1, m_cursor_position.y());
    }
}

HeadlessGUIScreen::CharLayout* HeadlessGUIScreen::cached_char_layout(cairo_scaled_font_t* font,
                                                                     char                 c) {
    auto cache_entry = m_char_layout_cache.find(c);
    if ( cache_entry != m_char_layout_cache.end() )
        return cache_entry->second;
    else {
        char char_buffer[] = { c, '\0' };
        auto char_layout   = new CharLayout{};

        cairo_text_cluster_flags_t text_cluster_flags;

        auto cairo_status = cairo_scaled_font_text_to_glyphs(font,
                                                             0,
                                                             0,
                                                             char_buffer,
                                                             1,
                                                             &char_layout->m_cairo_glyph,
                                                             &char_layout->m_glyph_count,
                                                             &char_layout->m_text_cluster,
                                                             &char_layout->m_cluster_count,
                                                             &text_cluster_flags);
        if ( cairo_status == CAIRO_STATUS_SUCCESS ) {
            m_char_layout_cache[c] = char_layout;
            return char_layout;
        } else
            return nullptr;
    }
}

bool HeadlessGUIScreen::char_is_utf8(char c) {
    return c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c <= 0x7E);
}

Graphics::Color::ArgbGradient HeadlessGUIScreen::screen_color_to_argb(ScreenColor screen_color) {
    switch ( screen_color ) {
        case SC_BLACK:
            return Graphics::Color::as_rgb(0, 0, 0);
        case SC_BLUE:
            return Graphics::Color::as_rgb(0, 0, 255);
        case SC_GREEN:
            return Graphics::Color::as_rgb(0, 255, 0);
        case SC_CYAN:
            return Graphics::Color::as_rgb(0, 255, 255);
        case SC_RED:
            return Graphics::Color::as_rgb(255, 0, 0);
        case SC_MAGENTA:
            return Graphics::Color::as_rgb(255, 0, 255);
        case SC_BROWN:
            return Graphics::Color::as_rgb(255, 248, 220);
        case SC_LGRAY:
            return Graphics::Color::as_rgb(211, 211, 211);
        case SC_DARKGRAY:
            return Graphics::Color::as_rgb(220, 220, 220);
        case SC_LBLUE:
            return Graphics::Color::as_rgb(129, 212, 250);
        case SC_LGREEN:
            return Graphics::Color::as_rgb(144, 238, 144);
        case SC_LCYAN:
            return Graphics::Color::as_rgb(224, 255, 255);
        case SC_LRED:
            return Graphics::Color::as_rgb(255, 127, 127);
        case SC_LMAGENTA:
            return Graphics::Color::as_rgb(231, 139, 231);
        case SC_YELLOW:
            return Graphics::Color::as_rgb(255, 255, 0);
        case SC_WHITE:
        default:
            return Graphics::Color::as_rgb(255, 255, 255);
    }
}
