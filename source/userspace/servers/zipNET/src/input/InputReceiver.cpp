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
#include <eva.h>
#include <io/keyboard.hpp>
#include <io/mouse.hpp>
#include <components/cursor.hpp>

/**
 *
 */
void InputReceiver_t::initialize()
{
	CreateThreadN((void*) InputReceiver_t::startReceiveMouseEvents, "keyReceiver");
	CreateThreadN((void*) InputReceiver_t::startReceiveKeyEvents, "mouseReceiver");
}

/**
 *
 */
void InputReceiver_t::startReceiveKeyEvents()
{
	EventProcessor_t *eventQueue = ZipNET::instance()->eventProcessor;

	Keyboard::Info info;
	while (true)
	{
		info = Keyboard::read();
		eventQueue->bufferKeyEvent(info);

		ZipNET::instance()->triggerRender();
	}
}

/**
 *
 */
void InputReceiver_t::startReceiveMouseEvents()
{
	ZipNET *instance = ZipNET::instance();
	EventProcessor_t *eventQueue = instance->eventProcessor;
	Dimension resolution = instance->videoOutput->getResolution();

	Mouse::Info info;
	while (true)
	{
		info = Mouse::read();

		Cursor_t::nextPosition.x += info.x;
		Cursor_t::nextPosition.y -= info.y;

		if (Cursor_t::nextPosition.x < 0) Cursor_t::nextPosition.x = 0;
		if (Cursor_t::nextPosition.x > resolution.width - 2) Cursor_t::nextPosition.x = resolution.width - 2;
		if (Cursor_t::nextPosition.y < 0) Cursor_t::nextPosition.y = 0;
		if (Cursor_t::nextPosition.y > resolution.height - 2) Cursor_t::nextPosition.y = resolution.height - 2;

		Cursor_t::nextPressedButtons = MOUSE_BUTTON_NONE;
		if (info.button1) Cursor_t::nextPressedButtons |= MOUSE_BUTTON_1;
		if (info.button2) Cursor_t::nextPressedButtons |= MOUSE_BUTTON_2;
		if (info.button3) Cursor_t::nextPressedButtons |= MOUSE_BUTTON_3;

		ZipNET::instance()->triggerRender();
	}
}
