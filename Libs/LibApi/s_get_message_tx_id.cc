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

#include <Api/User.h>

static bool                 g_next_transaction_lock = false;
static MessageTransaction g_next_transaction      = MESSAGE_TRANSACTION_FIRST;

MessageTransaction s_get_message_tx_id() {
    s_atomic_lock(&g_next_transaction_lock);

    /* safe obtain the next transaction identifier */
    MessageTransaction next_topic = g_next_transaction++;
    g_next_transaction_lock       = false;

    return next_topic;
}
