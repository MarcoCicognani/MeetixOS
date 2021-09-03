/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_API_CALLS_MESSAGING_CALLS
#define EVA_API_CALLS_MESSAGING_CALLS

#include <eva/ipc.h>

__BEGIN_C

/**
 * @field receiver
 * 		task id of the target task
 *
 * @field buffer
 * 		message buffer
 *
 * @field length
 * 		message length
 *
 * @field mode
 * 		sending mode
 *
 * @field status
 * 		one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
typedef struct {
    Tid                receiver;
    void*              buffer;
    size_t             length;
    MessageSendMode    mode;
    MessageTransaction transaction;

    MessageSendStatus status;
} __attribute__((packed)) SyscallSendMessage;

/**
 * @field buffer
 * 		target buffer
 *
 * @field maximum
 * 		buffer maximum length
 *
 * @field mode
 *		receiving mode
 *
 * @field transaction
 * 		transaction id or {MESSAGE_TRANSACTION_NONE}
 *
 * @field status
 * 		one of the {MessageReceiveStatus} codes
 *
 * @field break_condition
 * 		when giving, the receiving will wait no more
 * 		once the condition is true
 *
 * @security-level APPLICATION
 */
typedef struct {
    MessageHeader*     buffer;
    size_t             maximum;
    MessageReceiveMode mode;
    MessageTransaction transaction;
    uint8_t*           breakCondition;

    MessageReceiveStatus status;
} __attribute__((packed)) SyscallReceiveMessage;

__END_C

#endif
