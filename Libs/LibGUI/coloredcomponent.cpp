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

#include <gui/coloredcomponent.hpp>

/*
 *
 */
bool ColoredComponent::setColor(Graphics::Color::ArgbGradient shapeColor, Graphics::Color::ArgbGradient titleColor) {
    if ( !UiInitialized )
        return false;

    MessageTransaction tx = s_get_message_tx_id();

    UiComponentColor request;
    request.header.id  = UI_PROTOCOL_SET_COMPONENT_COLOR;
    request.id         = this->id;
    request.shapeColor = shapeColor;
    request.titleColor = titleColor;

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
