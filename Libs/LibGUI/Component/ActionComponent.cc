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
#include <LibGUI/Component/ActionComponent.hh>
#include <LibGUI/Component/Component.hh>
#include <LibGUI/Protocol.hh>
#include <stdio.h>

/**
 *
 */
bool ActionComponent::set_action_listener(ActionListener* l) {
    return self->set_listener(UI_COMPONENT_EVENT_TYPE_ACTION, l);
}
