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

MessageSendStatus s_send_message(Tid target, void* buffer, usize buffer_len) {
    return s_send_message_tm(target, buffer, buffer_len, MESSAGE_TRANSACTION_NONE, MESSAGE_SEND_MODE_BLOCKING);
}

MessageSendStatus s_send_message_m(Tid target, void* buffer, usize buffer_len, MessageSendMode send_mode) {
    return s_send_message_tm(target, buffer, buffer_len, MESSAGE_TRANSACTION_NONE, send_mode);
}

MessageSendStatus s_send_message_t(Tid target, void* buffer, usize buffer_len, MessageTransaction tx) {
    return s_send_message_tm(target, buffer, buffer_len, tx, MESSAGE_SEND_MODE_BLOCKING);
}

MessageSendStatus
s_send_message_tm(Tid target, void* buffer, usize buffer_len, MessageTransaction tx, MessageSendMode send_mode) {
    SyscallSendMessage data{ target, buffer, buffer_len, send_mode, tx };
    do_syscall(SYSCALL_MESSAGE_SEND, (usize)&data);
    return data.m_send_status;
}
