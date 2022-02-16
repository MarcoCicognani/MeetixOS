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
#include <LibGUI/Component/TextField.hh>
#include <LibGUI/Properties.hh>
#include <LibGUI/Protocol.hh>

/**
 *
 */
Textfield* Textfield::create() {
    return Component::create<Textfield, UI_COMPONENT_TYPE_TEXTFIELD>();
}

/**
 *
 */
void Textfield::setSecure(bool secure) {
    Component::set_numeric_property(UI_PROPERTY_SECURE, secure);
}

/**
 *
 */
bool Textfield::isSecure() {
    uint32_t value;
    Component::get_numeric_property(UI_PROPERTY_SECURE, &value);
    return value;
}
