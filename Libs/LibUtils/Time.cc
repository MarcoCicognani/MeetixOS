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

#include <Utils/Time.hh>

namespace Utils::Time {

bool current(Current& current) {
    /* obtain the thread-id of the time driver */
    auto time_tid = s_task_get_id(TIME_DRIVER_IDENTIFIER);
    if ( time_tid < 0 )
        return false;

    /* construct a get-current-time request */
    TimeDriverHeader request;
    request.m_command = CURRENT_TIME;

    auto message_tx = s_get_message_tx_id();

    /* send the message to the driver */
    auto send_status = s_send_message_t(time_tid, &request, sizeof(TimeDriverHeader), message_tx);
    if ( send_status != MESSAGE_SEND_STATUS_SUCCESSFUL )
        return false;

    /* receive the message */
    usize buffer_len = sizeof(MessageHeader) + sizeof(Current);
    u8    buffer[buffer_len];
    auto  recv_status = s_receive_message_t(buffer, buffer_len, message_tx);
    if ( recv_status == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        current = *reinterpret_cast<Current*>(MESSAGE_CONTENT(buffer));
        return true;
    }
    return false;
}

bool up_time(UpTime& up_time) {
    /* obtain the thread-id of the time driver */
    auto time_tid = s_task_get_id(TIME_DRIVER_IDENTIFIER);
    if ( time_tid < 0 )
        return false;

    /* construct a get-current-time request */
    TimeDriverHeader request;
    request.m_command = CURRENT_UPTIME;

    auto message_tx = s_get_message_tx_id();

    /* send the message to the driver */
    auto send_status = s_send_message_t(time_tid, &request, sizeof(TimeDriverHeader), message_tx);
    if ( send_status != MESSAGE_SEND_STATUS_SUCCESSFUL )
        return false;

    usize buffer_len = sizeof(MessageHeader) + sizeof(UpTime);
    u8    buffer[buffer_len];
    auto  recv_status = s_receive_message_t(buffer, buffer_len, message_tx);
    if ( recv_status == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
        up_time = *reinterpret_cast<UpTime*>(MESSAGE_CONTENT(buffer));
        return true;
    }
    return false;
}

} /* namespace Utils::Time */