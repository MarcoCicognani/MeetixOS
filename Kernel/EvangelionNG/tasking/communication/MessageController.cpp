/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#include <tasking/communication/MessageController.hpp>
#include <logger/logger.hpp>
#include "utils/HashMap.hpp"
#include "memory/memory.hpp"
#include "memory/collections/AddressStack.hpp"

/**
 * typedefs the map for message queue
 */
typedef HashMap<Tid, MessageQueueHead*> MessageQueueMap;

/**
 * message queues
 */
static MessageQueueMap *queues = 0;

/**
 * memory pool to store messages divided by size
 */
static AddressStack messageMemoryPool32;
static AddressStack messageMemoryPool64;
static AddressStack messageMemoryPool256;
static AddressStack messageMemoryPool2048;
static AddressStack messageMemoryPool4096;
static AddressStack messageMemoryPool8192;

/**
 * refill the adress stack with valid allocated addresses
 *
 * @param pool:		the AddressStack to fill
 * @param size:		the size of each allocation
 */
void refill(AddressStack *pool, size_t size)
{
	// fill each queue bucket
	for (int i = 0; i < MESSAGE_QUEUE_SIZE; i++)
	{
		// alloc the memory on the kernel heap
		Address addr = (Address) new uint8_t[size];

		// check validity
		if (!addr)
		{
			logInfo("%! out of memory while refilling memory pool %i", "messages", size);
			break;
		}

		// add to stack
		pool->push(addr);
	}
}

/**
 * get a valid address from provided pool, if pool is not filled it's auto refill
 *
 * @param pool:		the address stack where take the address
 * @param size:		the size of each bucket of the address
 */
Address getFromPool(AddressStack *pool, size_t size)
{
	// get an address
	Address addr = pool->pop();

	// check validity
	if (!addr)
	{
		// refill if stack is empty and recall this method
		refill(pool, size);
		return getFromPool(pool, size);
	}

	// return the address
	return addr;
}

/**
 * get a valid message header pointer of specified size from pools
 *
 * @param size:		the size of the message header
 * @return a pointer of type MessageHeader with size provided or null
 */
MessageHeader *get(size_t size)
{
	if (size < 32) return (MessageHeader*) getFromPool(&messageMemoryPool32, 32);
	else if (size < 64) return (MessageHeader*) getFromPool(&messageMemoryPool64, 64);
	else if (size < 256) return (MessageHeader*) getFromPool(&messageMemoryPool256, 256);
	else if (size < 2048) return (MessageHeader*) getFromPool(&messageMemoryPool2048, 2048);
	else if (size < 4096)return (MessageHeader*) getFromPool(&messageMemoryPool4096, 4096);
	else if (size < 8192)return (MessageHeader*) getFromPool(&messageMemoryPool8192, 8192);

	logInfo("%! invalid fill-pool requested, size %i", "messages", size);
	return 0;
}

/**
 * reuse the memory of the provided message adding it's address to the pool of same size
 *
 * @param msg:		the message to release
 */
void release(MessageHeader *msg)
{
	// calculate the size of the message
	size_t size = sizeof(MessageHeader) + msg->length;

	// readd the address to the corresponding pool
	if (size < 32) messageMemoryPool32.push((Address) msg);
	else if (size < 64) messageMemoryPool64.push((Address) msg);
	else if (size < 256) messageMemoryPool256.push((Address) msg);
	else if (size < 2048) messageMemoryPool2048.push((Address) msg);
	else if (size < 4096)messageMemoryPool4096.push((Address) msg);
	else if (size < 8192)messageMemoryPool8192.push((Address) msg);
	else logInfo("%! invalid released requested, size %i", "messages", size);
}

/**
 * Delete the message queue of the thread provided by id
 *
 * @param tid:		the identifier of the thread that is to be cleared
 */
void MessageController::clear(Tid tid)
{
	if (!queues) return;

	// get the queue
	MessageQueueMap::MapNode_t *entry = queues->get(tid);
	if (!entry) return;

	// release all messages
	MessageQueueHead *head = entry->value;
	MessageHeader *m = head->first;
	while (m)
	{
		MessageHeader *after = m->next;
		release(m);
		m = after;
	}

	// release the queue
	queues->erase(tid);
	delete head;
}

/**
 * Copy the <content> buffer provided by the <source> thread to the message queue of the <target> thread
 *
 * @param target:		the identifier of the thread that have to receive the message
 * @param source:		the identifier of the thread sender of the the message
 * @param contentLen:	the size of the buffer
 * @param tx:			the transaction identifier to distingue the messages
 */
MessageSendStatus MessageController::sendMessage(Tid target, Tid source, void *content, size_t contentLen, MessageTransaction tx)
{
	// ensure queue map
	if (!queues) queues = new MessageQueueMap();

	// check if message too long
	if (contentLen > MESSAGE_MAXIMUM_LENGTH) return MESSAGE_SEND_STATUS_EXCEEDS_MAXIMUM;

	// find/create queue head
	MessageQueueHead *queue;

	MessageQueueMap::MapNode_t *entry = queues->get(target);
	if (!entry)
	{
		queue = new MessageQueueHead();
		queues->add(target, queue);
	}

	else queue = entry->value;

	// check if it exceeds queue maximum
	if (queue->total + contentLen > MESSAGE_MAXIMUM_QUEUE_CONTENT) return MESSAGE_SEND_STATUS_QUEUE_FULL;

	// create message
	MessageHeader *message = get(sizeof(MessageHeader) + contentLen);
	if (!message) return MESSAGE_RECEIVE_STATUS_FAILED;
	message->transaction = tx;
	message->sender = source;
	message->length = contentLen;
	Memory::copy(MESSAGE_CONTENT(message), content, contentLen);

	// append to queue
	if (!queue->first) queue->first = message;
	if (queue->last) queue->last->next = message;
	message->previous = queue->last;
	message->next = 0;
	queue->last = message;

	// increment queue total content length
	queue->total += contentLen;

	return MESSAGE_SEND_STATUS_SUCCESSFUL;
}

/**
 * Copy the last received message on the message queue of the target on <out> buffer
 *
 * @param target:		the identifier of the thread that have to receive the message
 * @param out:			the pointer to the userspace instance of the MessageHeader where write the last message
 * @param max:			the size of the <out> buffer
 * @param tx:			the transaction identifier
 */
MessageReceiveStatus MessageController::receiveMessage(Tid target, MessageHeader *out, size_t max, MessageTransaction tx)
{
	// check for map
	if (!queues) return MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY;

	// find queue head
	MessageQueueHead *queue;

	MessageQueueMap::MapNode_t *entry = queues->get(target);
	if (!entry) return MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY;
	else queue = entry->value;

	if (!queue) return MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY;

	// find message
	MessageHeader *message = 0;

	if (tx == MESSAGE_TRANSACTION_NONE) message = queue->first;

	else
	{
		MessageHeader *n = queue->first;
		while (n)
		{
			if (n->transaction == tx)
			{
				message = n;
				break;
			}
			n = n->next;
		}
	}

	// no message?
	if (!message) return MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY;

	// check if message exceeds bounds
	size_t contentLen = message->length;
	if ((sizeof(MessageHeader) + contentLen) > max) return MESSAGE_RECEIVE_STATUS_EXCEEDS_BUFFER_SIZE;

	// copy message
	Memory::copy(out, message, sizeof(MessageHeader) + contentLen);

	// erase from queue
	if (message->next) message->next->previous = message->previous;
	else
	{
		queue->last = message->previous;
		if (queue->last) queue->last->next = 0;
	}

	if (message->previous) message->previous->next = message->next;
	else
	{
		queue->first = message->next;
		if (queue->first) queue->first->previous = 0;
	}

	// free the message
	release(message);

	// shrink queue total content length
	queue->total -= contentLen;
	return MESSAGE_RECEIVE_STATUS_SUCCESSFUL;
}
