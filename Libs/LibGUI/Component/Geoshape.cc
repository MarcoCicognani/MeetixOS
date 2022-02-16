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

#include <LibGUI/Component/Geoshape.hh>

/**
 *
 */
Geoshape* Geoshape::create() {
    return Component::create<Geoshape, UI_COMPONENT_TYPE_GEOSHAPE>();
}

/**
 *
 */
void Geoshape::set_resizable(bool resizable) {
    Component::set_numeric_property(UI_PROPERTY_RESIZABLE, resizable);
}
