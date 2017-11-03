/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
* 																			     *
* This program is free software; you can redistribute it and/or                  *
* modify it under the terms of the GNU General Public License                    *
* as published by the Free Software Foundation; either version 2				 *
* of the License, or (char *argumentat your option) any later version.			 *
*																				 *
* This program is distributed in the hope that it will be useful,				 *
* but WITHout ANY WARRANTY; without even the implied warranty of                 *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include <io/ps2.hpp>
#include <eva.h>
#include <stdio.h>

/**
 * shared area pointer
 */
Ps2SharedArea *ps2Area = nullptr;

/**
 * ready flag
 */
bool ps2IsRegistered = false;

/**
 * registration sincronizer flag
 */
static uint8_t ps2RegistrationLock = false;

/**
 * register the server client as requester of ps2 capabilities
 *
 * @return whether the registration success
 */
bool Ps2::registerSelf()
{
	AtomicLock(&ps2RegistrationLock);

	// if already registered return
	if (ps2IsRegistered) return true;

	// check the existance of the driver
	Tid driverTid = TaskGetID(PS2_DRIVER_IDENTIFIER);
	if (driverTid == -1)
	{
		klog("PS/2 driver registration failed: failed to identify PS/2 driver instance");
		ps2RegistrationLock = false;
		return false;
	}

	// send registration
	Ps2RegisterRequest request;
	MessageTransaction transaction = GetMessageTxId();
	if (SendMessageT(driverTid, &request, sizeof(Ps2RegisterRequest), transaction) != MESSAGE_SEND_STATUS_SUCCESSFUL)
	{
		klog("PS/2 driver registration error: failed to send registration request message");
		ps2RegistrationLock = false;
		return false;
	}

	// read response
	uint32_t responseBufLen = sizeof(MessageHeader) + sizeof(Ps2RegisterResponse);
	uint8_t responseBuffer[responseBufLen];
	if (ReceiveMessageT(responseBuffer, responseBufLen, transaction) != MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
	{
		klog("PS/2 driver registration error: failed to receive registration response message");
		ps2RegistrationLock = false;
		return false;
	}

	// receive content
	Ps2RegisterResponse *response = (Ps2RegisterResponse*) MESSAGE_CONTENT(responseBuffer);
	if (!response->area)
	{
		klog("PS/2 driver registration error: shared memory was NULL");
		ps2RegistrationLock = false;
		return false;
	}

	// all fine
	ps2RegistrationLock = false;
	ps2IsRegistered = true;
	ps2Area = response->area;
	return true;
}
