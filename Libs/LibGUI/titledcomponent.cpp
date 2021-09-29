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
#include <Api/utils/local.hpp>
#include <gui/titledcomponent.hpp>
#include <gui/ui.hpp>
#include <gui/uispech.hpp>
#include <cstring>

/**
 *
 */
bool TitledComponent::setTitle(std::string title) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    Local<UiComponentSetTitleRequest> request(new UiComponentSetTitleRequest());
    request()->header.id = UI_PROTOCOL_SET_TITLE;
    request()->id        = this->id;

    // fill text (truncate if necessary)
    size_t titleLen;
    if ( title.length() >= UI_COMPONENT_TITLE_MAXIMUM )
        titleLen = UI_COMPONENT_TITLE_MAXIMUM;

    else
        titleLen = title.length();
    memcpy(request()->title, title.c_str(), titleLen);
    request()->title[titleLen] = 0;

    s_send_message_t(UiDelegateTid, request(), sizeof(UiComponentSetTitleRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetTitleResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetTitleResponse* response
            = (UiComponentSetTitleResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
std::string TitledComponent::getTitle() {
    if ( !UiInitialized )
        return 0;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    UiComponentGetTitleRequest request;
    request.header.id = UI_PROTOCOL_GET_TITLE;
    request.id        = this->id;
    s_send_message_t(UiDelegateTid, &request, sizeof(UiComponentGetTitleRequest), tx);

    // read response
    size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiComponentGetTitleResponse);
    Local<uint8_t> buffer(new uint8_t[bufferSize]);

    if ( s_receive_message_t(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentGetTitleResponse* response
            = (UiComponentGetTitleResponse*)MESSAGE_CONTENT(buffer());
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return std::string(response->title);
    }

    return std::string();
}

/**
 *
 */
bool TitledComponent::setGhostTitle(std::string title) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    Local<UiComponentSetGhostTitleRequest> request(new UiComponentSetGhostTitleRequest());
    request()->header.id = UI_PROTOCOL_SET_GHOST_TITLE;
    request()->id        = this->id;

    // fill text (truncate if necessary)
    size_t titleLen;
    if ( title.length() >= UI_COMPONENT_TITLE_MAXIMUM )
        titleLen = UI_COMPONENT_TITLE_MAXIMUM;

    else
        titleLen = title.length();
    memcpy(request()->title, title.c_str(), titleLen);
    request()->title[titleLen] = 0;

    s_send_message_t(UiDelegateTid, request(), sizeof(UiComponentSetGhostTitleRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetGhostTitleResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetGhostTitleResponse* response
            = (UiComponentSetGhostTitleResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/*
 *
 */
bool TitledComponent::setFont(std::string fontName) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    Local<UiComponentSetTitleFontRequest> request(new UiComponentSetTitleFontRequest());
    request()->header.id = UI_PROTOCOL_SET_TITLE_FONT;
    request()->id        = this->id;

    // copy
    memcpy(request()->fontName, fontName.c_str(), fontName.length());

    s_send_message_t(UiDelegateTid, request(), sizeof(UiComponentSetTitleFontRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetTitleFontResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetTitleFontResponse* response
            = (UiComponentSetTitleFontResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/*
 *
 */
bool TitledComponent::setFontSize(int size) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    UiSetFontSizeRequest request;
    request.header.id = UI_PROTOCOL_SET_FONT_SIZE;
    request.id        = this->id;
    request.size      = size;

    s_send_message_t(UiDelegateTid, &request, sizeof(UiSetFontSizeRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiSetFontSizeResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiSetFontSizeResponse* response = (UiSetFontSizeResponse*)MESSAGE_CONTENT(buffer);

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/*
 *
 */
bool TitledComponent::setTitleAlignment(TextAlignment alignment) {
    if ( !UiInitialized )
        return false;

    // send initialization request
    MessageTransaction tx = s_get_message_tx_id();

    UiSetTitleAlignmentRequest request;
    request.header.id = UI_PROTOCOL_SET_TITLE_ALIGNMENT;
    request.id        = this->id;
    request.alignment = alignment;

    s_send_message_t(UiDelegateTid, &request, sizeof(UiSetTitleAlignmentRequest), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiSetTitleAlignmentResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiSetTitleAlignmentResponse* response
            = (UiSetTitleAlignmentResponse*)MESSAGE_CONTENT(buffer);

        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}

/**
 *
 */
UiComponentID TitledComponent::getId() {
    return id;
}
