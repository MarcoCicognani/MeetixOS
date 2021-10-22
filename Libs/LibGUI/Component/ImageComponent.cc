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

#include <cstring>
#include <GUI/Component/ImageComponent.hh>

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
    request.id        = this->id;
    memcpy(request.pathToPng, path.c_str(), pathLen);
    request.pathToPng[pathLen] = 0;
    request.pngPosition        = pngPosition;

    s_send_message_t(UiDelegateTid, &request, sizeof(UiComponentSetupPng), tx);

    // read response
    size_t  bufferSize = sizeof(MessageHeader) + sizeof(UiComponentSetupPngResponse);
    uint8_t buffer[bufferSize];

    if ( s_receive_message_t(buffer, bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        UiComponentSetupPngResponse* response
            = (UiComponentSetupPngResponse*)MESSAGE_CONTENT(buffer);
        if ( response->status == UI_PROTOCOL_SUCCESS )
            return true;
    }

    return false;
}
