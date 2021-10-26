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

#include <Graphics/Vbe.hh>

class HeadlessGUIScreen : public Screen {
public:
    ~HeadlessGUIScreen() override = default;

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
    int  color_foreground() override;
    void set_color_foreground(int c) override;
    int  color_background() override;

    void set_color_background(int c) override;

private:
    Graphics::Vbe::ModeInfo m_mode_info;
};
