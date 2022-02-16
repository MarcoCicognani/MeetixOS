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
#include <LibGUI/Component/Button.hh>
#include <LibGUI/Properties.hh>
#include <LibGUI/Protocol.hh>

/**
 *
 */
Button* Button::create() {
    return Component::create<Button, UI_COMPONENT_TYPE_BUTTON>();
}

/**
 *
 */
void Button::set_enabled(bool enabled) {
    Component::set_numeric_property(UI_PROPERTY_ENABLED, enabled);
}

/**
 *
 */
bool Button::is_enabled() {
    uint32_t value;
    Component::get_numeric_property(UI_PROPERTY_ENABLED, &value);
    return value;
}
