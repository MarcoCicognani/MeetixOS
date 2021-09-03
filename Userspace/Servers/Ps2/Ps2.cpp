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

#include "Ps2.hpp"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <string>
#include <eva.h>
#include <tasking/lock.hpp>
#include <utils/utils.hpp>
#include <io/ps2.hpp>

/**
 *
 */
uint8_t mousePacketNumber = 0;
uint8_t mousePacketBuffer[3];

/**
 * Receiver task ids and transaction ids for dispatching incoming packets.
 */
uint32_t mouseReceiverTid = -1;
uint32_t mouseReceiverTransaction;

uint32_t keyboardReceiverTid;
uint32_t keyboardReceiverTransaction;

/**
 *
 */
uint64_t packetsCount = 0;
Ps2SharedArea *sharedArea;

/**
 *
 */
int main()
{
	// register
	if (!RegisterAsServer(PS2_DRIVER_IDENTIFIER, SECURITY_LEVEL_DRIVER))
	{
		klog("Ps2Driver: failed to register as %s", PS2_DRIVER_IDENTIFIER);
		return 1;
	}

	// set up shared memory
	sharedArea = (Ps2SharedArea*) AllocMem(sizeof(Ps2SharedArea));
	if (!sharedArea)
	{
		Utils::log("failed to allocate transfer memory area");
		return 1;
	}

	// initialize memory area
	sharedArea->keyboard.atomNothingQueued = true;
	sharedArea->keyboard.atomUnhandled = false;
	sharedArea->mouse.atomNothingQueued = true;
	sharedArea->mouse.atomUnhandled = false;

	// initialize mouse
	initializeMouse();

	// start receiver threads
	registerOperationMode();

	// wait for control requests
	size_t buflen = sizeof(MessageHeader) + sizeof(Ps2RegisterRequest);
	uint8_t *buf = new uint8_t[buflen];
	Tid tid = GetTid();
	while (true)
	{
		MessageReceiveStatus stat = ReceiveMessage(buf, buflen);

		if (stat == MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
		{
			MessageHeader *mes = (MessageHeader*) buf;
			Ps2RegisterRequest *req = (Ps2RegisterRequest*) MESSAGE_CONTENT(buf);

			// share area with requester
			Pid requesterPid = GetPidForTid(mes->sender);
			Ps2SharedArea *sharedInTarget = (Ps2SharedArea*) ShareMem((void*) sharedArea, sizeof(Ps2SharedArea), requesterPid);

			// send response
			Ps2RegisterResponse response;
			response.area = sharedInTarget;
			SendMessageT(mes->sender, &response, sizeof(Ps2RegisterResponse), mes->transaction);
		}
	}
}

/**
 *
 */
void initializeMouse()
{
	uint8_t status;

	// empty input buffer
	while (Utils::Cpu::inportByte(0x64) & 0x01)
		Utils::Cpu::inportByte(0x60);

	// activate mouse device
	waitForBuffer(PS2_OUT);
	Utils::Cpu::outportByte(0x64, 0xA8);

	// get commando-byte, set bit 1 (enables IRQ12), send back
	waitForBuffer(PS2_OUT);
	Utils::Cpu::outportByte(0x64, 0x20);

	waitForBuffer(PS2_IN);
	status = Utils::Cpu::inportByte(0x60) | (1 << 1);

	waitForBuffer(PS2_OUT);
	Utils::Cpu::outportByte(0x64, 0x60);
	waitForBuffer(PS2_OUT);
	Utils::Cpu::outportByte(0x60, status);

	// send set-default-settings command to mouse
	writeToMouse(0xF6);

	waitForBuffer(PS2_IN);
	status = Utils::Cpu::inportByte(0x60);
	if (status != 0xFA)
	{
		Utils::log("mouse did not ack set-default-settings command");
		return;
	}

	// enable the mouse
	writeToMouse(0xF4);

	waitForBuffer(PS2_IN);
	status = Utils::Cpu::inportByte(0x60);
	if (status != 0xFA)
	{
		Utils::log("mouse did not ack enable-mouse command");
		return;
	}
}

/**
 *
 */
void handleMouseData(uint8_t b)
{
	switch (mousePacketNumber)
	{
	case 0:
		mousePacketBuffer[0] = b;

		// flags byte must have bit 3 set
		if ((b & (1 << 3)) == 0) mousePacketNumber = 0; // otherwise restart the cycle
		else mousePacketNumber = 1;

		break;

	case 1:
		mousePacketBuffer[1] = b;
		mousePacketNumber = 2;
		break;

	case 2:
		mousePacketBuffer[2] = b;

		int8_t flags = mousePacketBuffer[0];
		uint8_t valX = mousePacketBuffer[1];
		uint8_t valY = mousePacketBuffer[2];

		if ((flags & (1 << 6)) || (flags & (1 << 7)))
		{
			// ignore overflowing values
		}

		else
		{
			int16_t offX = valX - ((flags << 4) & 0x100);
			int16_t offY = valY - ((flags << 3) & 0x100);

			// wait for handling and set
			AtomicBlock(&sharedArea->mouse.atomUnhandled);

			// write data
			sharedArea->mouse.moveX = offX;
			sharedArea->mouse.moveY = offY;
			sharedArea->mouse.flags = flags;

			// show waiter that data is queue
			sharedArea->mouse.atomNothingQueued = false;
			sharedArea->mouse.atomUnhandled = true;
		}

		mousePacketNumber = 0;
		break;
	}
}

/**
 *
 */
void handleKeyboardData(uint8_t b)
{

	// wait for handling and set
	AtomicBlock(&sharedArea->keyboard.atomUnhandled);

	// write data
	sharedArea->keyboard.scancode = b;

	// show waiter that data is queue
	sharedArea->keyboard.atomNothingQueued = false;
	sharedArea->keyboard.atomUnhandled = true;
}

/**
 *
 */
void waitForBuffer(Ps2Buffer_t buffer)
{
	uint32_t timeout = 100000;

	if (buffer == PS2_OUT)
	{
		while (timeout--)
			if ((Utils::Cpu::inportByte(0x64) & 2) == 0) return;
	}

	else if (buffer == PS2_IN)
	{
		while (timeout--)
			if ((Utils::Cpu::inportByte(0x64) & 1) == 1) return;
	}
}

/**
 *
 */
void writeToMouse(uint8_t value)
{
	// tell the controller that we want to write to the mouse
	waitForBuffer(PS2_OUT);
	Utils::Cpu::outportByte(0x64, 0xD4);

	// send the value to the mouse
	waitForBuffer(PS2_OUT);
	Utils::Cpu::outportByte(0x60, value);
}
