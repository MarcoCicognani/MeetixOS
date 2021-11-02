/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <Api.h>
#include <Graphics/Vbe.hh>
#include <Utils/Utils.hh>

namespace Graphics::Vbe {

bool set_mode(u16 width, u16 height, u8 bpp, ModeInfo& mode_info) {
    /* find the thread-id of the driver */
    auto driver_id = s_task_get_id(VIDEO_DRIVER_IDENTIFIER);
    if ( driver_id < 0 ) {
        Utils::log("VBE driver registration failed: failed to identify VBE driver instance");
        return false;
    }

    /* obtain message transaction */
    auto message_tx = s_get_message_tx_id();

    /* request to the driver to set the requested mode */
    SetModeRequest request{ COMMAND_SET_MODE, width, height, bpp };
    auto send_status = s_send_message_t(driver_id, &request, sizeof(SetModeRequest), message_tx);
    if ( send_status != MESSAGE_SEND_STATUS_SUCCESSFUL ) {
        Utils::log("VBE driver communication error: failed to send set-mode request message");
        return false;
    }

    /* wait for incoming response */
    auto buffer_len = sizeof(MessageHeader) + sizeof(SetModeResponse);
    u8   buffer[buffer_len];
    auto recv_status = s_receive_message_t(buffer, buffer_len, message_tx);
    if ( recv_status != MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        Utils::log("VBE driver communication error: failed to receive set-mode request message");
        return false;
    }

    /* obtain the response */
    auto response = reinterpret_cast<SetModeResponse*>(MESSAGE_CONTENT(buffer));
    if ( response->m_mode_status != SET_MODE_STATUS_SUCCESS ) {
        Utils::log("VBE driver: Failed to set desired resolution");
        return false;
    }

    /* fill-out the mode-info */
    mode_info = response->m_mode_info;
    return true;
}

} /* namespace Graphics::Vbe */