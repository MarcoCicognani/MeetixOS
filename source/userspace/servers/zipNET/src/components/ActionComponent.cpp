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

#include <components/ActionComponent.hpp>
#include <components/component.hpp>
#include <stdio.h>

/**
 *
 */
void ActionComponent_t::fireAction()
{
	// if there is an internal handler, use it
	if (internalHandler)
	{
		internalHandler->handle(this);
		return;
	}

	// otherwise send message to registered thread
	EventListenerInfo_t listenerInfo;
	if (self->getListener(UI_COMPONENT_EVENT_TYPE_ACTION, listenerInfo))
	{
		UiComponentActionEvent actionEvent;
		actionEvent.header.type = UI_COMPONENT_EVENT_TYPE_ACTION;
		actionEvent.header.componentID = listenerInfo.componentID;
		SendMessage(listenerInfo.targetThread, &actionEvent, sizeof(UiComponentActionEvent));
	}
}

/**
 *
 */
void ActionComponent_t::setInternalActionHandler(InternalActionHandler_t *handler)
{
	this->internalHandler = handler;
}
