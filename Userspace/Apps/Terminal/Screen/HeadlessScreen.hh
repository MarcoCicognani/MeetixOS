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

#include "Screen.hh"

#include <LibTasking/Lock.hh>

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define VIDEO_MEMORY  0xB8000

class HeadlessScreen : public Screen {
public:
    HeadlessScreen()           = default;
    ~HeadlessScreen() override = default;

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
    void normalize();
    void update_visual_cursor() const;

private:
    Tasking::Lock m_lock{};
    u8*           m_output_current{ reinterpret_cast<u8*>(VIDEO_MEMORY) };
    u32           m_offset{ 0 };
    bool          m_is_cursor_visible{ true };
    bool          m_is_scroll_area_screen{ true };
    u32           m_scroll_area_start{ 0 };
    u32           m_scroll_area_end{ 0 };
};
