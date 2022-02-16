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

#include <LibGUI/Protocol.hh>
#include <string>
#include <vector>

class Listener;
class Component;
class Canvas;

/**
 *
 */
using UiOpenStatus                                     = int;
const UiOpenStatus UI_OPEN_STATUS_SUCCESSFUL           = 0;
const UiOpenStatus UI_OPEN_STATUS_COMMUNICATION_FAILED = 1;
const UiOpenStatus UI_OPEN_STATUS_FAILED               = 2;
const UiOpenStatus UI_OPEN_STATUS_EXISTING             = 3;

/*
 *
 */
using UiCloseStatus                            = int;
const UiCloseStatus UI_CLOSE_STATUS_SUCCESSFUL = 0;
const UiCloseStatus UI_CLOSE_STATUS_FAILED     = 1;
const UiCloseStatus UI_CLOSE_ALREADY_CLOSED    = 2;

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

/*
 * Declared in the UI unit
 */
extern bool UiInitialized;

/**
 *
 */
class UI {
private:
    [[noreturn]] static void event_dispatch_thread();

public:
    static UiOpenStatus                 open();
    static UiCloseStatus                close();
    static Graphics::Metrics::Dimension screen_dimension();

    static bool set_background(std::string path);
    static bool set_mouse_cursor(std::string name);

    static bool register_desktop_canvas(Canvas* c);
    static bool register_task_manager(Component* where, const Graphics::Metrics::Rectangle& bounds);
};
