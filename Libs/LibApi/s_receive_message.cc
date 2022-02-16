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

#include <Api/User.h>

MessageReceiveStatus s_receive_message(void* buffer, usize buffer_len) {
    return s_receive_message_tmb(buffer, buffer_len, MESSAGE_TRANSACTION_NONE, MESSAGE_RECEIVE_MODE_BLOCKING, nullptr);
}

MessageReceiveStatus s_receive_message_m(void* buffer, usize buffer_len, MessageReceiveMode receive_mode) {
    return s_receive_message_tmb(buffer, buffer_len, MESSAGE_TRANSACTION_NONE, receive_mode, nullptr);
}

MessageReceiveStatus s_receive_message_t(void* buffer, usize buffer_len, MessageTransaction tx) {
    return s_receive_message_tmb(buffer, buffer_len, tx, MESSAGE_RECEIVE_MODE_BLOCKING, nullptr);
}

MessageReceiveStatus
s_receive_message_tm(void* buffer, usize buffer_len, MessageTransaction tx, MessageReceiveMode receive_mode) {
    return s_receive_message_tmb(buffer, buffer_len, tx, receive_mode, nullptr);
}

MessageReceiveStatus s_receive_message_tmb(void*              buffer,
                                           usize              buffer_len,
                                           MessageTransaction tx,
                                           MessageReceiveMode receive_mode,
                                           bool*              break_condition) {
    SyscallReceiveMessage data{ reinterpret_cast<MessageHeader*>(buffer),
                                buffer_len,
                                receive_mode,
                                tx,
                                break_condition };
    do_syscall(SYSCALL_MESSAGE_RECEIVE, (usize)&data);
    return data.m_receive_status;
}
