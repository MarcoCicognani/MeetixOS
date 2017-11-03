/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                  *
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

#include <utils/time.hpp>

/*
 * fill time struct
 */
bool MXtime::getTime(TimeDriverCall &call)
{
	// identify time driver in task list
	Tid timeID = TaskGetID(TIME_DRIVER_IDENTIFIER);
	if (timeID == -1) return false;

	// get transaction
	MessageTransaction transaction = GetMessageTxId();

	//send message to timer server
	TimeDriverHeader request;
	request.command = GET_TIME;
	SendMessageT(timeID, &request, sizeof(TimeDriverHeader), transaction);

	// receive response
	size_t buflen = sizeof(MessageHeader) + sizeof(TimeDriverCall);
	uint8_t buf[buflen];

	if (ReceiveMessageT(buf, buflen, transaction) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
	{
		TimeDriverCall *content = (TimeDriverCall*) MESSAGE_CONTENT(buf);
		call.second = content->second;
		call.minute = content->minute;
		call.hour = content->hour;
		call.day = content->day;
		call.month = content->month;
		call.year = content->year;
		return true;
	}

	return false;
}

/*
 * fill uptime struct
 */
bool MXtime::getUptime(TimeDriverUptime &call)
{
	// identify time driver in task list
	Tid timeID = TaskGetID(TIME_DRIVER_IDENTIFIER);
	if (timeID == -1) return false;

	// get transaction
	MessageTransaction transaction = GetMessageTxId();

	//send message to timer server
	TimeDriverHeader request;
	request.command = GET_UPTIME;
	SendMessageT(timeID, &request, sizeof(TimeDriverHeader), transaction);

	// receive response
	size_t buflen = sizeof(MessageHeader) + sizeof(TimeDriverUptime);
	uint8_t buf[buflen];

	if (ReceiveMessageT(buf, buflen, transaction) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
	{
		TimeDriverUptime *content = (TimeDriverUptime*) MESSAGE_CONTENT(buf);
		call.second = content->second;

		return true;
	}

	return false;
}
