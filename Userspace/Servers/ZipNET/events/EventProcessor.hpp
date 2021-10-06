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

#ifndef __EVENT_PROCESSOR__
#define __EVENT_PROCESSOR__

#include <deque>
#include <gui/uispech.hpp>
#include <interface/CommandMessageResponderThread.hpp>
#include <IO/Keyboard.hh>
#include <IO/Mouse.hh>

using namespace std;

#define DEFAULT_MULTICLICK_TIMESPAN 250

/**
 * The event queue is used to store any incoming events for
 * later processing.
 */
class EventProcessor {
public:
    uint32_t multiclickTimespan;

    EventProcessor();

    deque<IO::Keyboard::Info> keyInfoBuffer;
    void                      bufferKeyEvent(IO::Keyboard::Info keyInfo);

    deque<void*> commandMessageBuffer;
    void         bufferCommandMessage(void* commandMessage);

    void process();
    void processCommand(Tid                       senderTid,
                        UiMessageHeader*          requestHeader,
                        CommandMessageResponse_t& responseOut);

    void translateKeyEvent(IO::Keyboard::Info& info);
    void processMouseState();
};

#endif
