/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_UI
#define MEETIX_LIBRARY_UI_UI

class Listener;
class Component;
class Canvas;

#include <gui/uispech.hpp>
#include <string>
#include <vector>

/**
 *
 */
typedef int        UiOpenStatus;
const UiOpenStatus UI_OPEN_STATUS_SUCCESSFUL           = 0;
const UiOpenStatus UI_OPEN_STATUS_COMMUNICATION_FAILED = 1;
const UiOpenStatus UI_OPEN_STATUS_FAILED               = 2;
const UiOpenStatus UI_OPEN_STATUS_EXISTING             = 3;

/*
 *
 */
typedef int         UiCloseStatus;
const UiCloseStatus UI_CLOSE_STATUS_SUCCESSFUL = 0;
const UiCloseStatus UI_CLOSE_STATUS_FAILED     = 1;
const UiCloseStatus UI_CLOSE_ALREADY_CLOSED    = 2;

/**
 *
 */
struct UiEventDispatchData {
    Listener* listener;
    uint8_t*  data;
    uint32_t  length;
};

/**
 * ID of the thread that the window server creates when
 * initializing the UI communication.
 */
extern Tid UiDelegateTid;

/**
 * ID of the event dispatcher thread that is continuously waiting
 * for events from the window manager to fire the respective listener
 * that was attached.
 */
extern Tid UiEventDispatcherTid;

/**
 *
 */
class UI {
private:
    [[noreturn]] static void eventDispatchThread();
    static void              eventDispatchQueueAdd(const UiEventDispatchData& data);

public:
    static UiOpenStatus  open();
    static UiCloseStatus close();
    static Dimension     getResolution();

    static bool setBackground(std::string path);
    static bool setMouseCursor(std::string name);

    static void addListener(Listener* l);
    static void removeListener(Listener* l);

    static bool registerDesktopCanvas(Canvas* c);
    static bool registerTaskManager(Component* where, const Rectangle& bounds);
};

#endif
