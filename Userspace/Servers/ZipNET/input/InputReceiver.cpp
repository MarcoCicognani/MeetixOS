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
#include <io/keyboard.hpp>
#include <io/mouse.hpp>

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

    Keyboard::Info info;
    while ( true ) {
        info = Keyboard::instance().read();
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
    Dimension       resolution = instance->videoOutput->getResolution();

    Mouse::Info info;
    while ( true ) {
        info = Mouse::read();

        Cursor::instance().nextPosition.x += info.x;
        Cursor::instance().nextPosition.y -= info.y;

        if ( Cursor::instance().nextPosition.x < 0 )
            Cursor::instance().nextPosition.x = 0;
        if ( Cursor::instance().nextPosition.x > resolution.width - 2 )
            Cursor::instance().nextPosition.x = resolution.width - 2;
        if ( Cursor::instance().nextPosition.y < 0 )
            Cursor::instance().nextPosition.y = 0;
        if ( Cursor::instance().nextPosition.y > resolution.height - 2 )
            Cursor::instance().nextPosition.y = resolution.height - 2;

        Cursor::instance().nextPressedButtons = MOUSE_BUTTON_NONE;
        if ( info.button1 )
            Cursor::instance().nextPressedButtons |= MOUSE_BUTTON_1;
        if ( info.button2 )
            Cursor::instance().nextPressedButtons |= MOUSE_BUTTON_2;
        if ( info.button3 )
            Cursor::instance().nextPressedButtons |= MOUSE_BUTTON_3;

        ZipNET::instance()->triggerRender();
    }
}
