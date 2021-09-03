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

#include "RegistrationThread.hpp"
#include <gui/uispech.hpp>
#include <interface/CommandMessageReceiverThread.hpp>
#include <interface/ApplicationExitCleanupThread.hpp>
#include <stdio.h>

/**
 *
 */
void RegistrationThread_t::run()
{
	// register this thread
	if (!TaskRegisterID("registration"))
	{
		klog("failed to register task identifier for registration thread");
		return;
	}

	// wait for initialization requests
	size_t bufferLength = sizeof(MessageHeader) + sizeof(UiInitializeRequest);
	uint8_t *buffer = new uint8_t[bufferLength];

	while (true)
	{
		MessageReceiveStatus stat = ReceiveMessage(buffer, bufferLength);

		if (stat == MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
		{
			MessageHeader *requestMessage = (MessageHeader*) buffer;
			UiInitializeRequest *request = (UiInitializeRequest*) MESSAGE_CONTENT(buffer);

			// create handler thread
			CommandMessageReceiverThread_t *communicator = new CommandMessageReceiverThread_t();
			Tid communicatorTid = communicator->start();

			// create a thread that cleans up the ui when the client thread exits
			ApplicationExitCleanupThread_t *cleanup = new ApplicationExitCleanupThread_t(GetPidForTid(requestMessage->sender), communicator);
			Tid cleaner = cleanup->start();

			// send response
			UiInitializeResponse response;
			response.header.id = UI_PROTOCOL_INITIALIZATION;
			response.status = UI_PROTOCOL_SUCCESS;
			response.windowServerDelegateThread = communicatorTid;
			response.windowServerCleanUPThread = cleaner;
			SendMessageT(requestMessage->sender, &response, sizeof(UiInitializeResponse), requestMessage->transaction);
		}
	}
}
