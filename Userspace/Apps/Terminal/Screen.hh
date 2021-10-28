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
#include <IO/Keyboard.hh>
#include <string>

typedef u8 ScreenColor;

#define SC_BLACK    0x0
#define SC_BLUE     0x1
#define SC_GREEN    0x2
#define SC_CYAN     0x3
#define SC_RED      0x4
#define SC_MAGENTA  0x5
#define SC_BROWN    0x6
#define SC_LGRAY    0x7
#define SC_DARKGRAY 0x8
#define SC_LBLUE    0x9
#define SC_LGREEN   0xA
#define SC_LCYAN    0xB
#define SC_LRED     0xC
#define SC_LMAGENTA 0xD
#define SC_YELLOW   0xE
#define SC_WHITE    0xF

#define SC_COLOR(ba, fo) static_cast<u8>((fo) | ((ba) << 4))

#define VT100_COLOR_BLACK   0
#define VT100_COLOR_RED     1
#define VT100_COLOR_GREEN   2
#define VT100_COLOR_YELLOW  3
#define VT100_COLOR_BLUE    4
#define VT100_COLOR_MAGENTA 5
#define VT100_COLOR_CYAN    6
#define VT100_COLOR_WHITE   7

class Screen {
public:
    Screen()          = default;
    virtual ~Screen() = default;

    virtual IO::Keyboard::Info read_input() = 0;

    virtual void clean()                   = 0;
    virtual void backspace()               = 0;
    virtual void write_char(char c)        = 0;
    virtual void move_cursor(int x, int y) = 0;

    virtual int cursor_x() = 0;
    virtual int cursor_y() = 0;
    virtual int width()    = 0;
    virtual int height()   = 0;

    virtual void set_scroll_area_screen()            = 0;
    virtual void set_scroll_area(int start, int end) = 0;
    virtual void scroll(int value)                   = 0;

    virtual void set_cursor_visible(bool visible) = 0;

    [[nodiscard]] ScreenColor color_foreground() const {
        return m_color_foreground;
    }
    void set_color_foreground(int c) {
        m_color_foreground = c;
    }

    [[nodiscard]] ScreenColor color_background() const {
        return m_color_background;
    }
    void set_color_background(int c) {
        m_color_background = c;
    }

private:
    ScreenColor m_color_foreground{ SC_WHITE };
    ScreenColor m_color_background{ SC_BLACK };
};
