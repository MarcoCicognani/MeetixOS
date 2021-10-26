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

#include "HeadlessScreen.hh"

#include <cstring>
#include <Tasking/LockGuard.hh>
#include <Utils/Utils.hh>

HeadlessScreen::HeadlessScreen() {
    clean();
}

IO::Keyboard::Info HeadlessScreen::read_input() {
    return IO::Keyboard::instance().read();
}

void HeadlessScreen::clean() {
    Tasking::LockGuard lock_guard{ m_lock };

    for ( auto offset = 0; offset < SCREEN_HEIGHT * SCREEN_WIDTH * 2; offset += 2 ) {
        m_output_current[offset]     = ' ';
        m_output_current[offset + 1] = SC_COLOR(SC_BLACK, SC_WHITE);
    }
    m_offset = 0;
}

void HeadlessScreen::backspace() {
    Tasking::LockGuard lock_guard{ m_lock };

    m_offset -= 2;
    m_output_current[m_offset] = ' ';

    /* re-position BIOS cursor */
    update_visual_cursor();
}

void HeadlessScreen::write_char(char c) {
    Tasking::LockGuard lock_guard{ m_lock };

    if ( c == '\n' ) {
        m_offset += SCREEN_WIDTH * 2;
        m_offset -= m_offset % (SCREEN_WIDTH * 2);
    } else {
        m_output_current[m_offset++] = c;
        m_output_current[m_offset++] = SC_COLOR(color_background(), color_foreground());
    }

    /* ensure valid offset */
    normalize();

    /* re-position BIOS cursor */
    update_visual_cursor();
}

void HeadlessScreen::move_cursor(int x, int y) {
    Tasking::LockGuard lock_guard{ m_lock };

    m_offset = (y * SCREEN_WIDTH * 2) + x * 2;
    if ( m_offset < 0 )
        m_offset = 0;
    else if ( m_offset > (SCREEN_WIDTH * 2 * SCREEN_HEIGHT) )
        m_offset = SCREEN_WIDTH * 2 * SCREEN_HEIGHT;

    /* re-position BIOS cursor */
    update_visual_cursor();
}

int HeadlessScreen::cursor_x() {
    return static_cast<int>((m_offset % (SCREEN_WIDTH * 2)) / 2);
}

int HeadlessScreen::cursor_y() {
    return static_cast<int>(m_offset / (SCREEN_WIDTH * 2));
}

int HeadlessScreen::width() {
    return SCREEN_WIDTH;
}

int HeadlessScreen::height() {
    return SCREEN_HEIGHT;
}

void HeadlessScreen::set_scroll_area_screen() {
    Tasking::LockGuard lock_guard{ m_lock };

    m_is_scroll_area_screen = true;
}

void HeadlessScreen::set_scroll_area(int start, int end) {
    Tasking::LockGuard lock_guard{ m_lock };

    m_is_scroll_area_screen = false;
    m_scroll_area_start     = start > SCREEN_HEIGHT ? SCREEN_HEIGHT : start;
    m_scroll_area_end       = end > SCREEN_HEIGHT ? SCREEN_HEIGHT : end;
}

void HeadlessScreen::scroll(int value) {
    Tasking::LockGuard lock_guard{ m_lock };

    auto scroll_start = m_is_scroll_area_screen ? 0 : m_scroll_area_start;
    auto scroll_end   = m_is_scroll_area_screen ? SCREEN_HEIGHT : m_scroll_area_end;

    if ( value > 0 ) {
        for ( auto i = scroll_end - 2; i >= scroll_start; --i )
            std::memcpy(&m_output_current[(i + value) * SCREEN_WIDTH * 2],
                        &m_output_current[i * SCREEN_WIDTH * 2],
                        SCREEN_WIDTH * 2);
    } else {
        for ( auto i = scroll_start; i < scroll_end; ++i )
            std::memcpy(&m_output_current[i * SCREEN_WIDTH * 2],
                        &m_output_current[(i + value) * SCREEN_WIDTH * 2],
                        SCREEN_WIDTH * 2);
    }
}

void HeadlessScreen::set_cursor_visible(bool visible) {
    Tasking::LockGuard lock_guard{ m_lock };

    m_is_cursor_visible = visible;
    if ( m_is_cursor_visible )
        update_visual_cursor();
    else {
        Utils::PortIO::write_u8(0x3D4, 0x0F);
        Utils::PortIO::write_u8(0x3D5, (u8)(-1 & 0xFF));
        Utils::PortIO::write_u8(0x3D4, 0x0E);
        Utils::PortIO::write_u8(0x3D5, (u8)((-1 >> 8) & 0xFF));
    }
}

void HeadlessScreen::normalize() {
    Tasking::LockGuard lock_guard{ m_lock };

    if ( m_offset >= SCREEN_WIDTH * SCREEN_HEIGHT * 2 ) {
        m_offset = m_offset - SCREEN_WIDTH * 2;

        auto line_bytes  = SCREEN_WIDTH * 2;
        auto screen_size = SCREEN_HEIGHT * line_bytes;

        std::memcpy(m_output_current,
                    &m_output_current[SCREEN_WIDTH * 2],
                    screen_size - line_bytes);

        for ( auto i = 0; i < SCREEN_WIDTH * 2; i += 2 ) {
            m_output_current[screen_size - line_bytes + i]     = ' ';
            m_output_current[screen_size - line_bytes + i + 1] = SC_BLACK;
        }
    }
}

void HeadlessScreen::update_visual_cursor() {
    Tasking::LockGuard lock_guard{ m_lock };

    auto x = (m_offset % (SCREEN_WIDTH * 2)) / 2;
    auto y = m_offset / (SCREEN_WIDTH * 2);

    auto position = (y * SCREEN_WIDTH) + x;

    Utils::PortIO::write_u8(0x3D4, 0x0F);
    Utils::PortIO::write_u8(0x3D5, (u8)(position & 0xFF));
    Utils::PortIO::write_u8(0x3D4, 0x0E);
    Utils::PortIO::write_u8(0x3D5, (u8)((position >> 8) & 0xFF));
}
