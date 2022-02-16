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

#include <LibGUI/Component/ColoredComponent.hh>

/*
 *
 */
bool ColoredComponent::set_color(Graphics::Color::ArgbGradient shapeColor, Graphics::Color::ArgbGradient titleColor) {
    if ( !UiInitialized )
        return false;

    MessageTransaction tx = s_get_message_tx_id();

    UiComponentColor request;
    request.header.m_command = UI_PROTOCOL_SET_COMPONENT_COLOR;
    request.id               = this->id;
    request.shapeColor       = shapeColor;
    request.titleColor       = titleColor;

    s_send_message_t(UiDelegateTid, &request, sizeof(UiComponentColor), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentColorResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentColorResponse* response = (UiComponentColorResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS ) {
            return true;
        }
    }

    return false;
}
