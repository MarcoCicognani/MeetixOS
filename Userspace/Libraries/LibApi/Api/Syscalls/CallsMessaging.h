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

#pragma once

#include <Api/IPC.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief s_send_message system call data
 */
typedef struct {
    Tid                m_receiver_thread_id;
    void*              m_in_buffer;
    usize              m_in_buffer_len;
    MessageSendMode    m_send_mode;
    MessageTransaction m_message_transaction;
    MessageSendStatus  m_send_status;
} A_PACKED SyscallSendMessage;

/**
 * @brief s_receive_message system call data
 */
typedef struct {
    MessageHeader*       m_out_buffer;
    size_t               m_out_buffer_len;
    MessageReceiveMode   m_receive_mode;
    MessageTransaction   m_message_transaction;
    bool*                m_break_condition;
    MessageReceiveStatus m_receive_status;
} A_PACKED SyscallReceiveMessage;

#ifdef __cplusplus
}
#endif
