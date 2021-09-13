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

#include <eva.h>
#include <gui/component.hpp>
#include <gui/uispech.hpp>
#include <utils/valplacer.hpp>

/*
 *
 */
Component::~Component() {
    removeFromZipNetRegex();
    ComponentRegistry::remove(this->id);
}

/*
 *
 */
void Component::removeFromZipNetRegex() {
    if ( !UiInitialized )
        return;

    // send remove request
    MessageTransaction tx = GetMessageTxId();

    UiRemoveComponentRequest request;
    request.header.id = UI_PROTOCOL_REMOVE_COMPONENT;
    request.id        = this->id;
    SendMessageT(UiDelegateTid, &request, sizeof(UiRemoveComponentRequest), tx);
}

/**
 *
 */
bool Component::addChild(Component* child) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentAddChildRequest request;
    request.header.id = UI_PROTOCOL_ADD_COMPONENT;
    request.parent    = this->id;
    request.child     = child->id;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentAddChildRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentAddChildResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentAddChildResponse* response
            = (UiComponentAddChildResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool Component::setBounds(Rectangle rect) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentSetBoundsRequest request;
    request.header.id = UI_PROTOCOL_SET_BOUNDS;
    request.id        = this->id;
    request.bounds    = rect;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentSetBoundsRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetBoundsResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetBoundsResponse* response
            = (UiComponentSetBoundsResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
Rectangle Component::getBounds() {
    if ( !UiInitialized )
        return Rectangle();

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentGetBoundsRequest request;
    request.header.id = UI_PROTOCOL_GET_BOUNDS;
    request.id        = this->id;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentGetBoundsRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiComponentGetBoundsResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentGetBoundsResponse* response
            = (UiComponentGetBoundsResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return response->bounds;
    }

    return Rectangle();
}

/**
 *
 */
bool Component::setVisible(bool visible) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentSetVisibleRequest request;
    request.header.id = UI_PROTOCOL_SET_VISIBLE;
    request.id        = this->id;
    request.visible   = visible;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentSetVisibleRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetVisibleResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetVisibleResponse* response
            = (UiComponentSetVisibleResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool Component::setFocus(bool focus) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentFocusRequest request;
    request.header.id = UI_PROTOCOL_SET_FOCUS;
    request.id        = this->id;
    request.focus     = focus;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentFocusRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentFocusResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentFocusResponse* response = (UiComponentFocusResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool Component::setNumericProperty(int property, uint32_t value) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentSetNumericPropertyRequest request;
    request.header.id = UI_PROTOCOL_SET_NUMERIC_PROPERTY;
    request.id        = this->id;
    request.property  = property;
    request.value     = value;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentSetNumericPropertyRequest), tx);

    // read response
    size_t bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetNumericPropertyResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetNumericPropertyResponse* response
            = (UiComponentSetNumericPropertyResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool Component::getNumericProperty(int property, uint32_t* out) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiComponentGetNumericPropertyRequest request;
    request.header.id = UI_PROTOCOL_GET_NUMERIC_PROPERTY;
    request.id        = this->id;
    request.property  = property;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentGetNumericPropertyRequest), tx);

    // read response
    size_t bufferSize = sizeof(MessageHeader) + sizeof(UiComponentGetNumericPropertyResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentGetNumericPropertyResponse* response
            = (UiComponentGetNumericPropertyResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS ) {
            *out = response->value;
            return true;
        }
    }

    return false;
}

/**
 *
 */
bool Component::setListener(UiComponentEventType eventType, Listener* newListener) {
    if ( !UiInitialized )
        return false;

    // set new
    listeners[eventType] = newListener;

    // check
    if ( newListener == nullptr )
        return false;

    // send request
    MessageTransaction tx = GetMessageTxId();

    UiComponentSetListenerRequest request;
    request.header.id    = UI_PROTOCOL_SET_LISTENER;
    request.id           = this->id;
    request.targetThread = UiEventDispatcherTid;
    request.eventType    = eventType;
    SendMessageT(UiDelegateTid, &request, sizeof(UiComponentSetListenerRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetListenerResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetListenerResponse* response
            = (UiComponentSetListenerResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
void Component::handle(UiComponentEventHeader* header) {
    auto      eventType = header->type;
    Listener* listener  = listeners[eventType];

    if ( listener != nullptr )
        listener->process(header);
}

/**
 *
 */
bool Component::setMouseListener(MouseListener* listener) {
    return setListener(UI_COMPONENT_EVENT_TYPE_MOUSE, listener);
}

/**
 *
 */
bool Component::setLayout(UiLayoutManager layout) {
    return setNumericProperty(UI_PROPERTY_LAYOUT_MANAGER, layout);
}
