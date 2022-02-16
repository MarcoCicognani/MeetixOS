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
#include <Api/utils/local.hpp>
#include <cstring>
#include <deque>
#include <LibGraphics/Metrics/Dimension.hh>
#include <LibGUI/Application.hh>
#include <LibGUI/Component/ActionComponent.hh>
#include <LibGUI/Component/Canvas.hh>
#include <LibGUI/Component/Component.hh>
#include <LibGUI/Listener/ActionListener.hh>
#include <LibGUI/Listener/Listener.hh>
#include <LibUtils/Utils.hh>
#include <map>
#include <sstream>
#include <stdio.h>

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

    // get window managers m_command
    Tid windowMgr = s_task_get_id(C_REGISTRATION_THREAD_NAME);
    if ( windowMgr == -1 ) {
        Utils::log("failed to retrieve task m_command of window server with identifier '%s'",
                   (char*)C_REGISTRATION_THREAD_NAME);
        return UI_OPEN_STATUS_COMMUNICATION_FAILED;
    }

    // start event dispatcher
    UiEventDispatcherTid = s_create_thread_n((void*)&event_dispatch_thread, "UI::event_dispatch_thread");

    // send initialization request
    MessageTransaction initTx = s_get_message_tx_id();

    UiInitializeRequest request;
    request.header.m_command = UI_PROTOCOL_INITIALIZATION;
    s_send_message_t(windowMgr, &request, sizeof(UiInitializeRequest), initTx);

    // receive initialization response
    uint32_t       responseBufferSize = sizeof(MessageHeader) + sizeof(UiInitializeResponse);
    Local<uint8_t> responseBuffer(new uint8_t[responseBufferSize]);
    if ( s_receive_message_t(responseBuffer(), responseBufferSize, initTx) != MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
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
    s_kill(cleanUpThread);

    // delete the component object of client map
    // (the object deletion remove itself from component map and from zipNet regex)
    ComponentRegistry::instance().destroy();

    // get m_command of transaction
    MessageTransaction tx = s_get_message_tx_id();

    // fill message and send request
    UiRemoveComponentMapRequest request;
    request.header.m_command = UI_PROTOCOL_REMOVE_COMPONENT_MAP;
    s_send_message_t(UiDelegateTid, &request, sizeof(UiRemoveComponentMapRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiRemoveComponentMapResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    // get the message
    if ( s_receive_message_t(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiRemoveComponentMapResponse* response = (UiRemoveComponentMapResponse*)MESSAGE_CONTENT(buffer());

        // close only if response from ZipNET is succeded
        if ( response->status == UI_PROTOCOL_SUCCESS ) {
            // kill the interface communication threads
            s_kill(UiEventDispatcherTid);

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
Graphics::Metrics::Dimension UI::screen_dimension() {
    if ( !UiInitialized )
        return Graphics::Metrics::Dimension();

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    UiGetResolution request;
    request.header.m_command = UI_PROTOCOL_GET_RESOLUTION;

    s_send_message_t(UiDelegateTid, &request, sizeof(UiGetResolution), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiGetResolutionResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiGetResolutionResponse* response = (UiGetResolutionResponse*)MESSAGE_CONTENT(buffer);
        return response->resolution;
    }

    return Graphics::Metrics::Dimension();
}

/**
 *
 */
bool UI::set_background(std::string path) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    UiChangeBackgroundRequest request;
    request.header.m_command = UI_PROTOCOL_CHANGE_BACKGROUND;
    strncpy(request.path, path.c_str(), PATH_MAX);

    s_send_message_t(UiDelegateTid, &request, sizeof(UiChangeBackgroundRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiChangeBackgroundResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiChangeBackgroundResponse* response = (UiChangeBackgroundResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool UI::set_mouse_cursor(std::string name) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    // fill message
    UiSetMouseCursorFormRequest request;
    request.header.m_command = UI_PROTOCOL_SET_MOUSE_CURSOR_FORM;
    strncpy(request.name, name.c_str(), 50);

    // send the message
    s_send_message_t(UiDelegateTid, &request, sizeof(UiSetMouseCursorFormRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiSetMouseCursorFormResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiSetMouseCursorFormResponse* response = (UiSetMouseCursorFormResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
[[noreturn]] void UI::event_dispatch_thread() {
    size_t         buffer_size = C_MESSAGE_LEN_MAX;
    Local<uint8_t> buffer(new uint8_t[buffer_size]);

    while ( true ) {
        int stat = s_receive_message(buffer(), buffer_size);
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
bool UI::register_desktop_canvas(Canvas* c) {
    if ( !UiInitialized )
        return false;

    MessageTransaction tx = s_get_message_tx_id();

    // send registration request
    UiRegisterDesktopCanvasRequest request;
    request.header.m_command = UI_PROTOCOL_REGISTER_DESKTOP_CANVAS;
    request.canvasID         = c->id();
    s_send_message_t(UiDelegateTid, &request, sizeof(UiRegisterDesktopCanvasRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiRegisterDesktopCanvasResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( s_receive_message_t(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiRegisterDesktopCanvasResponse* response = (UiRegisterDesktopCanvasResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
bool UI::register_task_manager(Component* where, const Graphics::Metrics::Rectangle& bounds) {
    if ( !UiInitialized )
        return false;

    MessageTransaction tx = s_get_message_tx_id();

    // send registration
    UiRegisterTaskManagerRequest request;
    request.header.m_command = UI_PROTOCOL_REGISTER_TASK_MANAGER;
    request.id               = where->id();
    request.bounds           = bounds;
    s_send_message_t(UiDelegateTid, &request, sizeof(UiRegisterTaskManagerRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiRegisterTaskManagerResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( s_receive_message_t(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiRegisterTaskManagerResponse* response = (UiRegisterTaskManagerResponse*)MESSAGE_CONTENT(buffer());

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}
