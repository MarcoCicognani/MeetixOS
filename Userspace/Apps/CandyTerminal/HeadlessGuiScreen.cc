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

#include "HeadlessGuiScreen.hh"

#include <Graphics/Vbe.hh>

bool HeadlessGUIScreen::init() {
    return Graphics::Vbe::set_mode(1024, 768, 32, m_mode_info);
}

IO::Keyboard::Info HeadlessGUIScreen::read_input() {
    return {};
}

void HeadlessGUIScreen::clean() {
}

void HeadlessGUIScreen::backspace() {
}

void HeadlessGUIScreen::write_char(char c) {
}

void HeadlessGUIScreen::move_cursor(int x, int y) {
}

int HeadlessGUIScreen::cursor_x() {
    return 0;
}

int HeadlessGUIScreen::cursor_y() {
    return 0;
}

int HeadlessGUIScreen::width() {
    return 0;
}

int HeadlessGUIScreen::height() {
    return 0;
}

void HeadlessGUIScreen::set_scroll_area_screen() {
}

void HeadlessGUIScreen::set_scroll_area(int start, int end) {
}

void HeadlessGUIScreen::scroll(int value) {
}

void HeadlessGUIScreen::set_cursor_visible(bool visible) {
}

int HeadlessGUIScreen::color_foreground() {
    return Screen::color_foreground();
}

void HeadlessGUIScreen::set_color_foreground(int c) {
    Screen::set_color_foreground(c);
}

int HeadlessGUIScreen::color_background() {
    return Screen::color_background();
}

void HeadlessGUIScreen::set_color_background(int c) {
    Screen::set_color_background(c);
}
