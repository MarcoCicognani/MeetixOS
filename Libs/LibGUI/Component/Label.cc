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
#include <LibGUI/Component/Label.hh>
#include <LibGUI/Protocol.hh>

/**
 *
 */
Label* Label::create() {
    return Component::create<Label, UI_COMPONENT_TYPE_LABEL>();
}
