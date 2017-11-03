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

#include <gui/uispech.hpp>
#include <interface/CommandMessageReceiverThread.hpp>
#include <stdio.h>
#include <zipNET.hpp>
#include <string.h>

/**
 *
 */
void CommandMessageReceiverThread_t::run()
{
	TaskRegisterID("messageReceiver");

	// create a buffer for incoming command messages
	size_t bufferLength = sizeof(MessageHeader) + UI_MAXIMUM_MESSAGE_SIZE;
	uint8_t *buffer = new uint8_t[bufferLength];

	while (!stop)
	{
		// receive messages
		MessageReceiveStatus stat = ReceiveMessageTMB(buffer, bufferLength, MESSAGE_TRANSACTION_NONE, MESSAGE_RECEIVE_MODE_BLOCKING, &stop);

		if (stop) break;

		if (stat == MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
		{
			MessageHeader *requestMessage = (MessageHeader*) buffer;

			// add message to the event processing queue
			size_t messageTotalSize = sizeof(MessageHeader) + requestMessage->length;
			uint8_t *messageCopy = new uint8_t[messageTotalSize];
			memcpy(messageCopy, buffer, messageTotalSize);

			ZipNET::instance()->eventProcessor->bufferCommandMessage(messageCopy);
			ZipNET::instance()->triggerRender();

		}

		else if (stat == MESSAGE_RECEIVE_STATUS_EXCEEDS_BUFFER_SIZE) klog("could not receive an incoming request, message exceeded buffer size");
		else klog("an unknown error occurred when trying to receive a UI request (code: %i)", stat);
	}

	delete buffer;
}
