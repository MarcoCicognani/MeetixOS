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

#include <deque>
#include <eva.h>
#include <eva/utils/local.hpp>
#include <graphics/metrics/dimension.hpp>
#include <gui/actioncomponent.hpp>
#include <gui/actionlistener.hpp>
#include <gui/canvas.hpp>
#include <gui/component.hpp>
#include <gui/listener.hpp>
#include <gui/ui.hpp>
#include <map>
#include <sstream>
#include <stdio.h>
#include <utils/utils.hpp>
#include <utils/valplacer.hpp>

/**
 * Global ready indicator
 */
bool UiInitialized = false;

/**
 *
 */
Tid UiDelegateTid        = -1;
Tid cleanUpThread        = -1;
Tid UiEventDispatcherTid = -1;

/**
 * Opens a connection to the window server.
 */
UiOpenStatus UI::open() {
    // check if already open
    if ( UiInitialized )
        return UI_OPEN_STATUS_EXISTING;

    // get window managers id
    Tid windowMgr = TaskGetID(UI_REGISTRATION_THREAD_IDENTIFIER);
    if ( windowMgr == -1 ) {
        Utils::log("failed to retrieve task id of window server with identifier '%s'",
                   (char*)UI_REGISTRATION_THREAD_IDENTIFIER);
        return UI_OPEN_STATUS_COMMUNICATION_FAILED;
    }

    // start event dispatcher
    UiEventDispatcherTid = CreateThreadN((void*)&eventDispatchThread, "UI::eventDispatchThread");

    // send initialization request
    MessageTransaction initTx = GetMessageTxId();

    UiInitializeRequest request;
    request.header.id = UI_PROTOCOL_INITIALIZATION;
    SendMessageT(windowMgr, &request, sizeof(UiInitializeRequest), initTx);

    // receive initialization response
    uint32_t       responseBufferSize = sizeof(MessageHeader) + sizeof(UiInitializeResponse);
    Local<uint8_t> responseBuffer(new uint8_t[responseBufferSize]);
    if ( ReceiveMessageT(responseBuffer(), responseBufferSize, initTx)
         != MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        Utils::log("failed to communicate with the window server");
        return UI_OPEN_STATUS_COMMUNICATION_FAILED;
    }

    // check response
    auto response = (UiInitializeResponse*)MESSAGE_CONTENT(responseBuffer());
    if ( response->status != UI_PROTOCOL_SUCCESS ) {
        Utils::log("failed to open UI");
        return UI_OPEN_STATUS_FAILED;
    }

    // mark UI as ready
    UiInitialized = true;
    UiDelegateTid = response->windowServerDelegateThread;
    cleanUpThread = response->windowServerCleanUPThread;
    return UI_OPEN_STATUS_SUCCESSFUL;
}

/*
 * terminate the connection to windowserver and erase al components objects
 */
UiCloseStatus UI::close() {
    // check if already closed
    if ( !UiInitialized )
        return UI_CLOSE_ALREADY_CLOSED;

    // kill the thread for clean up of windowserver because the process
    // already cleanup the environment
    Kill(cleanUpThread);

    // delete the component object of client map
    // (the object deletion remove itself from component map and from zipNet regex)
    ComponentRegistry::instance().deleteRegistry();

    // get id of transaction
    MessageTransaction tx = GetMessageTxId();

    // fill message and send request
    UiRemoveComponentMapRequest request;
    request.header.id = UI_PROTOCOL_REMOVE_COMPONENT_MAP;
    SendMessageT(UiDelegateTid, &request, sizeof(UiRemoveComponentMapRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiRemoveComponentMapResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    // get the message
    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiRemoveComponentMapResponse* response
            = (UiRemoveComponentMapResponse*)MESSAGE_CONTENT(buffer());

        // close only if response from ZipNET is succeded
        if ( response->status == UI_PROTOCOL_SUCCESS ) {
            // kill the interface communication threads
            Kill(UiEventDispatcherTid);

            // uninitialize UI
            UiInitialized = false;

            // from this point the interface connection is closed and the graphic
            // application can only terminate or swith to text mode
            return UI_CLOSE_STATUS_SUCCESSFUL;
        }
    }

    return UI_CLOSE_STATUS_FAILED;
}

/**
 *
 */
Dimension UI::getResolution() {
    if ( !UiInitialized )
        return Dimension();

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiGetResolution request;
    request.header.id = UI_PROTOCOL_GET_RESOLUTION;

    SendMessageT(UiDelegateTid, &request, sizeof(UiGetResolution), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiGetResolutionResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiGetResolutionResponse* response = (UiGetResolutionResponse*)MESSAGE_CONTENT(buffer);
        return response->resolution;
    }

    return Dimension();
}

/**
 *
 */
bool UI::setBackground(std::string path) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    UiChangeBackgroundRequest request;
    request.header.id = UI_PROTOCOL_CHANGE_BACKGROUND;
    strncpy(request.path, path.c_str(), MAX_PATH);

    SendMessageT(UiDelegateTid, &request, sizeof(UiChangeBackgroundRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiChangeBackgroundResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiChangeBackgroundResponse* response = (UiChangeBackgroundResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool UI::setMouseCursor(std::string name) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = GetMessageTxId();

    // fill message
    UiSetMouseCursorFormRequest request;
    request.header.id = UI_PROTOCOL_SET_MOUSE_CURSOR_FORM;
    strncpy(request.name, name.c_str(), 50);

    // send the message
    SendMessageT(UiDelegateTid, &request, sizeof(UiSetMouseCursorFormRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiSetMouseCursorFormResponse);
    uint8_t buffer[bufferSize];

    if ( ReceiveMessageT(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiSetMouseCursorFormResponse* response
            = (UiSetMouseCursorFormResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
[[noreturn]] void UI::eventDispatchThread() {
    size_t         buffer_size = UI_MAXIMUM_MESSAGE_SIZE;
    Local<uint8_t> buffer(new uint8_t[buffer_size]);

    while ( true ) {
        int stat = ReceiveMessage(buffer(), buffer_size);
        if ( stat == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            // event message
            auto eventHeader = (UiComponentEventHeader*)MESSAGE_CONTENT(buffer());
            auto component   = ComponentRegistry::instance().get(eventHeader->componentID);

            if ( !component ) {
                Utils::log("event received for unknown component %i", eventHeader->componentID);
                continue;
            }

            // tell the component delegate to handle the event
            component->handle(eventHeader);

        } else {
            Utils::log("something went wrong when receiving an event, status code: %i", stat);
        }
    }
}

/**
 *
 */
bool UI::registerDesktopCanvas(Canvas* c) {
    if ( !UiInitialized )
        return false;

    MessageTransaction tx = GetMessageTxId();

    // send registration request
    UiRegisterDesktopCanvasRequest request;
    request.header.id = UI_PROTOCOL_REGISTER_DESKTOP_CANVAS;
    request.canvasID  = c->getID();
    SendMessageT(UiDelegateTid, &request, sizeof(UiRegisterDesktopCanvasRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiRegisterDesktopCanvasResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiRegisterDesktopCanvasResponse* response
            = (UiRegisterDesktopCanvasResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool UI::registerTaskManager(Component* where, const Rectangle& bounds) {
    if ( !UiInitialized )
        return false;

    MessageTransaction tx = GetMessageTxId();

    // send registration
    UiRegisterTaskManagerRequest request;
    request.header.id = UI_PROTOCOL_REGISTER_TASK_MANAGER;
    request.id        = where->getID();
    request.bounds    = bounds;
    SendMessageT(UiDelegateTid, &request, sizeof(UiRegisterTaskManagerRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiRegisterTaskManagerResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiRegisterTaskManagerResponse* response
            = (UiRegisterTaskManagerResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}
