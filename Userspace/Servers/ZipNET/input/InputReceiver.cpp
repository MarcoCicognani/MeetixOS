/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "InputReceiver.hpp"

#include <Api.h>
#include <components/cursor.hpp>
#include <IO/Keyboard.hh>
#include <IO/Mouse.hh>

/**
 *
 */
void InputReceiver::initialize() {
    s_create_thread_n((void*)InputReceiver::startReceiveMouseEvents, "keyReceiver");
    s_create_thread_n((void*)InputReceiver::startReceiveKeyEvents, "mouseReceiver");
}

/**
 *
 */
void InputReceiver::startReceiveKeyEvents() {
    EventProcessor* eventQueue = ZipNET::instance()->eventProcessor;

    IO::Keyboard::Info info;
    while ( true ) {
        info = IO::Keyboard::instance().read();
        eventQueue->bufferKeyEvent(info);

        ZipNET::instance()->triggerRender();
    }
}

/**
 *
 */
void InputReceiver::startReceiveMouseEvents() {
    ZipNET*         instance   = ZipNET::instance();
    EventProcessor* eventQueue = instance->eventProcessor;
    auto            resolution = instance->videoOutput->getResolution();

    IO::Mouse::Info info;
    while ( true ) {
        info = IO::Mouse::instance().read();

        Cursor::instance().nextPosition.set_x(Cursor::instance().nextPosition.x() + info.m_x);
        Cursor::instance().nextPosition.set_y(Cursor::instance().nextPosition.y() - info.m_y);

        if ( Cursor::instance().nextPosition.x() < 0 )
            Cursor::instance().nextPosition.set_x(0);
        if ( Cursor::instance().nextPosition.x() > resolution.width() - 2 )
            Cursor::instance().nextPosition.set_x( resolution.width() - 2);
        if ( Cursor::instance().nextPosition.y() < 0 )
            Cursor::instance().nextPosition.set_y(0);
        if ( Cursor::instance().nextPosition.y() > resolution.height() - 2 )
            Cursor::instance().nextPosition.set_y(resolution.height() - 2);

        Cursor::instance().nextPressedButtons = MOUSE_BUTTON_NONE;
        if ( info.m_button1 )
            Cursor::instance().nextPressedButtons |= MOUSE_BUTTON_1;
        if ( info.m_button2 )
            Cursor::instance().nextPressedButtons |= MOUSE_BUTTON_2;
        if ( info.m_button3 )
            Cursor::instance().nextPressedButtons |= MOUSE_BUTTON_3;

        ZipNET::instance()->triggerRender();
    }
}
