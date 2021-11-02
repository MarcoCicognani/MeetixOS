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
#include <IO/Input.hh>
#include <Tasking/LockGuard.hh>
#include <Utils/Utils.hh>

namespace IO {

Input& Input::instance() {
    static Input* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new Input{};
    return *s_instance;
}

bool Input::register_self() {
    Tasking::LockGuard guard{ m_lock };

    /* if already registered return */
    if ( is_registered() )
        return true;

    /* find the thread-id of the driver */
    auto driver_id = s_task_get_id(INPUT_DRIVER_IDENTIFIER);
    if ( driver_id < 0 ) {
        Utils::log("PS/2 driver registration failed: failed to identify PS/2 driver instance");
        return false;
    }

    /* obtain message transaction */
    auto message_tx = s_get_message_tx_id();

    /* request to the driver to register us */
    RegisterRequest request;
    auto send_status = s_send_message_t(driver_id, &request, sizeof(RegisterRequest), message_tx);
    if ( send_status != MESSAGE_SEND_STATUS_SUCCESSFUL ) {
        Utils::log("PS/2 driver registration error: failed to send registration request message");
        return false;
    }

    /* wait for incoming response */
    auto buffer_len = sizeof(MessageHeader) + sizeof(RegisterResponse);
    u8   buffer[buffer_len];
    auto recv_status = s_receive_message_t(buffer, buffer_len, message_tx);
    if ( recv_status != MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        Utils::log(
            "PS/2 driver registration error: failed to receive registration response message");
        return false;
    }

    /* extract the content */
    auto response = reinterpret_cast<RegisterResponse*>(MESSAGE_CONTENT(buffer));
    if ( !response->m_shared_area ) {
        Utils::log("PS/2 driver registration error: shared memory was nullptr");
        return false;
    }

    /* update the global values */
    m_is_registered = true;
    m_shared_area   = response->m_shared_area;
    return true;
}

} /* namespace IO */