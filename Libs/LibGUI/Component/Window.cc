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

#include <Api.h>
#include <LibGUI/Application.hh>
#include <LibGUI/Component/Window.hh>
#include <LibGUI/Properties.hh>
#include <LibGUI/Protocol.hh>

/**
 *
 */
class WindowCloseListener : public Listener {
private:
    void (*stdFunctionToClose)();

public:
    WindowCloseListener(void (*func)()) { stdFunctionToClose = func; }

    virtual void process(UiComponentEventHeader* header) { stdFunctionToClose(); }
};

/**
 *
 */
Window* Window::create() {
    return Component::create<Window, UI_COMPONENT_TYPE_WINDOW>();
}

/**
 *
 */
bool Window::is_resizable() {
    uint32_t value;
    Component::get_numeric_property(UI_PROPERTY_RESIZABLE, &value);
    return value;
}

/**
 *
 */
void Window::set_resizable(bool resizable) {
    Component::set_numeric_property(UI_PROPERTY_RESIZABLE, resizable);
}

/**
 *
 */
bool Window::on_close(void (*func)()) {
    return set_listener(UI_COMPONENT_EVENT_TYPE_CLOSE, new WindowCloseListener(func));
}
