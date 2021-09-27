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

#include <Api.h>
#include <gui/properties.hpp>
#include <gui/ui.hpp>
#include <gui/uispech.hpp>
#include <gui/window.hpp>

/**
 *
 */
class WindowCloseListener : public Listener {
private:
    void (*stdFunctionToClose)();

public:
    WindowCloseListener(void (*func)()) {
        stdFunctionToClose = func;
    }

    virtual void process(UiComponentEventHeader* header) {
        stdFunctionToClose();
    }
};

/**
 *
 */
Window* Window::create() {
    return createComponent<Window, UI_COMPONENT_TYPE_WINDOW>();
}

/**
 *
 */
bool Window::isResizable() {
    uint32_t value;
    Component::getNumericProperty(UI_PROPERTY_RESIZABLE, &value);
    return value;
}

/**
 *
 */
void Window::setResizable(bool resizable) {
    Component::setNumericProperty(UI_PROPERTY_RESIZABLE, resizable);
}

/**
 *
 */
bool Window::onClose(void (*func)()) {
    return setListener(UI_COMPONENT_EVENT_TYPE_CLOSE, new WindowCloseListener(func));
}
