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

#include "Screen.hh"

#include <cairo/cairo.h>
#include <Graphics/Color.hh>
#include <Graphics/Context.hh>
#include <Graphics/Text/Font.hh>
#include <Graphics/Video.hh>
#include <Tasking/Lock.hh>
#include <Tasking/Thread.hh>
#include <Utils/Utils.hh>

class HeadlessGUIScreen : public Screen {
    class PainterThread : public Tasking::Thread {
    public:
        PainterThread(HeadlessGUIScreen& gui_screen)
            : Tasking::Thread{ "GUIPainter" }
            , m_gui_screen{ gui_screen } {
        }
        ~PainterThread() override = default;

    protected:
        void run() override {
            m_gui_screen.paint();
        }

    private:
        HeadlessGUIScreen& m_gui_screen;
    };

    struct CharLayout {
    public:
        cairo_glyph_t*        m_cairo_glyph{ nullptr };
        int                   m_glyph_count{};
        cairo_text_cluster_t* m_text_cluster{ nullptr };
        int                   m_cluster_count{};
    };

    struct RasterCell {
    public:
        char                          m_char{ '\0' };
        Graphics::Color::ArgbGradient m_background{ screen_color_to_argb(SC_BLACK) };
        Graphics::Color::ArgbGradient m_foreground{ screen_color_to_argb(SC_WHITE) };

        RasterCell() = default;
        RasterCell(char c, ScreenColor background, ScreenColor foreground)
            : m_char{ c }
            , m_background{ screen_color_to_argb(background) }
            , m_foreground{ screen_color_to_argb(foreground) } {
        }

        operator bool() const {
            return m_char != '\0';
        }
    };

public:
    ~HeadlessGUIScreen() override {
        delete m_painter_thread;
        delete[] m_raster_buffer;
    }

    bool init();

    IO::Keyboard::Info read_input() override;

    void clean() override;
    void backspace() override;
    void write_char(char c) override;
    void move_cursor(int x, int y) override;

    int cursor_x() override;
    int cursor_y() override;
    int width() override;
    int height() override;

    void set_scroll_area_screen() override;
    void set_scroll_area(int start, int end) override;
    void scroll(int value) override;

    void set_cursor_visible(bool visible) override;

private:
    [[noreturn]] void paint();

    void blit_to_screen() const;
    void repaint();

    void move_cursor_unlocked(int x, int y);
    void write_char_unlocked(char c);

    CharLayout& cached_char_layout(cairo_scaled_font_t* font, char c);

    static bool                          char_is_utf8(char c);
    static Graphics::Color::ArgbGradient screen_color_to_argb(ScreenColor screen_color);

private:
    Graphics::Video::ModeInfo    m_vbe_mode_info{};
    Graphics::Context            m_back_context{};
    Graphics::Metrics::Point     m_cursor_position{ 0, 0 };
    Graphics::Metrics::Dimension m_font_dimension{ 8, 12 };
    Graphics::Text::Font*        m_font{ nullptr };
    Tasking::Lock                m_raster_buffer_lock{};
    Tasking::Lock                m_render_lock{};
    RasterCell*                  m_raster_buffer{ nullptr };
    PainterThread*               m_painter_thread{ nullptr };
    std::map<char, CharLayout>   m_char_layout_cache{};
    u64                          m_last_input_ts{ 0 };
};
