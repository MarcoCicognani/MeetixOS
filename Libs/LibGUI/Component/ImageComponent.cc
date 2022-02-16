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

#include <cstring>
#include <LibGUI/Component/ImageComponent.hh>

/**
 *
 */
bool ImageComponent::set_image(std::string path, Graphics::Metrics::Point pngPosition) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    size_t pathLen;
    if ( path.length() > PATH_MAX )
        pathLen = PATH_MAX;

    else
        pathLen = path.length();

    UiComponentSetupPng request;
    request.header.m_command = UI_PROTOCOL_SET_PNG;
    request.id               = this->id;
    memcpy(request.pathToPng, path.c_str(), pathLen);
    request.pathToPng[pathLen] = 0;
    request.pngPosition        = pngPosition;

    s_send_message_t(UiDelegateTid, &request, sizeof(UiComponentSetupPng), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetupPngResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetupPngResponse* response = (UiComponentSetupPngResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}
