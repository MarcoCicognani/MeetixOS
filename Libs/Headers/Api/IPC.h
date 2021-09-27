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

#pragma once

#include <Api/Common.h>
#include <Api/Kernel.h>
#include <Api/StdInt.h>

__BEGIN_C

/**
 * @brief Message transaction identifier
 */
typedef usize MessageTransaction;
#define MESSAGE_TRANSACTION_NONE  ((MessageTransaction)0)
#define MESSAGE_TRANSACTION_FIRST ((MessageTransaction)1)

/**
 * @brief Message header automatically prepended to the buffer by the kernel
 */
typedef struct MessageHeader {
    Tid                   m_sender_tid;
    MessageTransaction    m_transaction;
    usize                 m_message_len;
    struct MessageHeader* m_previous;
    struct MessageHeader* m_next;
} A_PACKED MessageHeader;

/**
 * @brief Used to extract the pointer to the message received
 */
#define MESSAGE_CONTENT(message) (((u8*)(message)) + sizeof(MessageHeader))

/**
 * @brief Message limits
 */
#define MESSAGE_MAXIMUM_LENGTH        (8192)
#define MESSAGE_MAXIMUM_QUEUE_CONTENT (8192 * 64)

/**
 * @brief Message send modes
 */
typedef enum
{
    MESSAGE_SEND_MODE_BLOCKING,
    MESSAGE_SEND_MODE_NON_BLOCKING
} MessageSendMode;

/**
 * @brief Message receive modes
 */
typedef enum
{
    MESSAGE_RECEIVE_MODE_BLOCKING,
    MESSAGE_RECEIVE_MODE_NON_BLOCKING
} MessageReceiveMode;

/**
 * @brief Message send statuses
 */
typedef enum
{
    MESSAGE_SEND_STATUS_SUCCESSFUL,
    MESSAGE_SEND_STATUS_QUEUE_FULL,
    MESSAGE_SEND_STATUS_FAILED,
    MESSAGE_SEND_STATUS_EXCEEDS_MAXIMUM
} MessageSendStatus;

/**
 * @brief Message receive statuses
 */
typedef enum
{
    MESSAGE_RECEIVE_STATUS_SUCCESSFUL,
    MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY,
    MESSAGE_RECEIVE_STATUS_FAILED,
    MESSAGE_RECEIVE_STATUS_FAILED_NOT_PERMITTED,
    MESSAGE_RECEIVE_STATUS_EXCEEDS_BUFFER_SIZE,
    MESSAGE_RECEIVE_STATUS_INTERRUPTED
} MessageReceiveStatus;

__END_C
