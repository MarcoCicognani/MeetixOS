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
#include <LibGUI/Component/Canvas.hh>
#include <LibGUI/Listener/CanvasWFAListener.hh>
#include <LibGUI/Protocol.hh>

/**
 *
 */
Canvas::~Canvas() {
    delete graphics;
    delete userListener;
}

/**
 *
 */
CanvasBufferInfo Canvas::buffer_info() {
    CanvasBufferInfo info;

    if ( nextBuffer ) {
        // unmap old buffer if available
        if ( currentBuffer )
            s_unmap_mem((void*)currentBuffer);

        // store new one
        currentBuffer = nextBuffer;

        // tell server we have acknowledged the changed buffer
        MessageTransaction tx = s_get_message_tx_id();

        UiComponentCanvasAckBufferRequest request;
        request.header.m_command = UI_PROTOCOL_CANVAS_ACK_BUFFER_REQUEST;
        request.id        = this->m_component_id;
        s_send_message_t(UiDelegateTid, &request, sizeof(UiComponentCanvasAckBufferRequest), tx);

        nextBuffer = 0;
    }

    if ( currentBuffer == 0 )
        info.buffer = 0;

    else {
        // return buffer
        info.buffer
            = (Graphics::Color::ArgbGradient*)(currentBuffer + UI_CANVAS_SHARED_MEMORY_HEADER_SIZE);
        auto header = (UiCanvasSharedMemoryHeader*)currentBuffer;
        info.width  = header->paintableWidth;
        info.height = header->paintableHeight;
    }
    return info;
}

/**
 *
 */
Canvas* Canvas::create() {
    auto instance = Component::create<Canvas, UI_COMPONENT_TYPE_CANVAS>();
    if ( instance )
        instance->set_listener(UI_COMPONENT_EVENT_TYPE_CANVAS_WFA, new CanvasWfaListener(instance));

    return instance;
}

/**
 *
 */
void Canvas::acknowledge_new_buffer(Address address) {
    if ( address == currentBuffer )
        return;
    nextBuffer = address;

    if ( userListener )
        userListener->handle_buffer_changed();
}

/**
 *
 */
void Canvas::blit(Graphics::Metrics::Rectangle rect) {
    if ( currentBuffer == 0 )
        return;

    // write blit parameters
    auto header        = (UiCanvasSharedMemoryHeader*)currentBuffer;
    header->blitX      = rect.x();
    header->blitY      = rect.y();
    header->blitWidth  = rect.width();
    header->blitHeight = rect.height();
    header->is_ready   = true;

    // send blit message
    MessageTransaction tx = s_get_message_tx_id();

    UiComponentCanvasBlitRequest request;
    request.header.m_command = UI_PROTOCOL_CANVAS_BLIT;
    request.id        = this->m_component_id;
    s_send_message_t(UiDelegateTid, &request, sizeof(UiComponentCanvasBlitRequest), tx);
}
