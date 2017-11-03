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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include <eva.h>
#include <gui/uispech.hpp>
#include <gui/canvas.hpp>
#include <gui/canvaswfalistener.hpp>

/**
 *
 */
Canvas::~Canvas()
{
	delete graphics;
	delete userListener;
}

/**
 *
 */
CanvasBufferInfo Canvas::getBuffer()
{
	CanvasBufferInfo info;

	if (nextBuffer)
	{
		// unmap old buffer if available
		if (currentBuffer) Unmap((void*) currentBuffer);

		// store new one
		currentBuffer = nextBuffer;

		// tell server we have acknowledged the changed buffer
		MessageTransaction tx = GetMessageTxId();

		UiComponentCanvasAckBufferRequest request;
		request.header.id = UI_PROTOCOL_CANVAS_ACK_BUFFER_REQUEST;
		request.id = this->id;
		SendMessageT(UiDelegateTid, &request, sizeof(UiComponentCanvasAckBufferRequest), tx);

		nextBuffer = 0;
	}

	if (currentBuffer == 0) info.buffer = 0;

	else
	{
		// return buffer
		info.buffer = (Color_t*) (currentBuffer + sizeof(UiCanvasSharedMemoryHeader));
		UiCanvasSharedMemoryHeader *header = (UiCanvasSharedMemoryHeader*) currentBuffer;
		info.width = header->paintableWidth;
		info.height = header->paintableHeight;

	}
	return info;
}

/**
 *
 */
Canvas *Canvas::create()
{
	Canvas *instance = createComponent<Canvas, UI_COMPONENT_TYPE_CANVAS>();

	if (instance) instance->setListener(UI_COMPONENT_EVENT_TYPE_CANVAS_WFA, new CanvasWfaListener(instance));

	return instance;
}

/**
 *
 */
void Canvas::acknowledgeNewBuffer(Address address)
{
	if (address == currentBuffer) return;
	nextBuffer = address;

	if (userListener) userListener->handleBufferChanged();
}

/**
 *
 */
void Canvas::blit(Rectangle rect)
{
	if (currentBuffer == 0) return;

	// write blit parameters
	UiCanvasSharedMemoryHeader *header = (UiCanvasSharedMemoryHeader*) currentBuffer;
	header->blitX = rect.x;
	header->blitY = rect.y;
	header->blitWidth = rect.width;
	header->blitHeight = rect.height;

	// send blit message
	MessageTransaction tx = GetMessageTxId();

	UiComponentCanvasBlitRequest request;
	request.header.id = UI_PROTOCOL_CANVAS_BLIT;
	request.id = this->id;
	SendMessageT(UiDelegateTid, &request, sizeof(UiComponentCanvasBlitRequest), tx);
}
