/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#include "cmos.hpp"
#include <stdio.h>

TIME var;

/**
 * ask to bios if there are updates
 */
int Cmos_t::getUpdateInProgressFlag()
{
    Utils::Cpu::outportByte(cmosAddress, 0x0A);
    return (Utils::Cpu::inportByte(cmosData) & 0x80);
}

/**
 * return index of rtc registers
 */
uint8_t Cmos_t::getRTCregister(int reg)
{
    Utils::Cpu::outportByte(cmosAddress, reg);
    return Utils::Cpu::inportByte(cmosData);
}

/**
 *  with register index read the rtc and fill the variable
 */
void Cmos_t::readRTC()
{
    size_t currentYear = 2016;

    uint8_t century;
    uint8_t last_second;
    uint8_t last_minute;
    uint8_t last_hour;
    uint8_t last_day;
    uint8_t last_month;
    uint8_t last_year;
    uint8_t last_century;
    uint8_t registerB;
    int time[3];

    int centuryRegister = 0x00; // Set by ACPI table parsing code if possible

    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (getUpdateInProgressFlag()); // Make sure an update isn't in progress

    var.second   =   getRTCregister(0x00);
    var.minute   =   getRTCregister(0x02);
    var.hour     =   getRTCregister(0x04);
    var.day      =   getRTCregister(0x07);
    var.month    =   getRTCregister(0x08);
    var.year     =   getRTCregister(0x09);

    if (centuryRegister != 0) var.century = getRTCregister(centuryRegister);

    do
    {
        last_second   =  var.second;
        last_minute   =  var.minute;
        last_hour     =  var.hour;
        last_day      =  var.day;
        last_month    =  var.month;
        last_year     =  var.year;
        last_century  =  var.century;

        while (getUpdateInProgressFlag());           // Make sure an update isn't in progress

        var.second  =  getRTCregister(0x00);
        var.minute  =  getRTCregister(0x02);
        var.hour    =  getRTCregister(0x04);
        var.day     =  getRTCregister(0x07);
        var.month   =  getRTCregister(0x08);
        var.year    =  getRTCregister(0x09);

        if (centuryRegister != 0) var.century = getRTCregister(centuryRegister);

    }
    while ((last_second == var.second) && (last_minute == var.minute) && (last_hour == var.hour) && (last_day == var.day) && (last_month == var.month) && (last_year == var.year) && (last_century == var.century));

    registerB = getRTCregister(0x0B);

    // Convert BCD to binary values if necessary
    if (!(registerB & 0x04))
    {
        var.second  =  (var.second & 0x0F) + ((var.second / 16) * 10);
        var.minute  =  (var.minute & 0x0F) + ((var.minute / 16) * 10);
        var.hour    =  ((var.hour & 0x0F) + (((var.hour & 0x70) / 16) * 10) ) | (var.hour & 0x80);
        var.day     =  (var.day & 0x0F) + ((var.day / 16) * 10);
        var.month   =  (var.month & 0x0F) + ((var.month / 16) * 10);
        var.year    =  (var.year & 0x0F) + ((var.year / 16) * 10);

        if (centuryRegister != 0) var.century = (var.century & 0x0F) + ((var.century / 16) * 10);
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(registerB & 0x02) && (time[2] & 0x80)) time[2] = ((time[2] & 0x7F) + 12) % 24;

    // Calculate the full (4-digit) year
    if (centuryRegister != 0) var.year = var.year + var.century * 100;

    else
    {
        var.year = var.year + (currentYear / 100) * 100;

        if(var.year < currentYear) var.year = var.year + 100;
    }
}

/**
 * thread for timer, increase variables of global time
 */
void Cmos_t::timer(Cmos_t *instance)
{
    // read the bios
    instance->readRTC();
    Utils::log("time: %i/%i/%i %i:%i:%i", var.day, var.month, var.year, var.hour + 2, var.minute, var.second);

    // mainloop of thread
	while (true)
	{
        // wait each second
		Sleep(1000);

        // increase or decrease variables
        var.second++;
        var.upSecond++;
		if (var.second > 59) {
			var.second = 0;
			var.minute++;

			if (var.minute > 59) {
				var.minute = 0;
				var.hour++;
			}
		}
	}
}

/**
 *
 */
int main(int argc, char *argv[])
{
    if (!RegisterAsServer(TIME_DRIVER_IDENTIFIER, SECURITY_LEVEL_DRIVER))
    {
        klog("TimeDriver: could not register with task identifier %s", TIME_DRIVER_IDENTIFIER);
        return -1;
    }

    // create object to cmos
	Cmos_t *EvaTime = new Cmos_t();

    // create timer thread
	CreateThreadDN((void*) &Cmos_t::timer, (void*) EvaTime, "timer");

    // set length for buffer
	const size_t buflen = sizeof(MessageHeader) + sizeof(TimeDriverHeader);

    // read another time, (the thread read first)
    EvaTime->readRTC();

    // mainloop
	while (true)
	{
        // create buffer
        uint8_t buf[buflen];

        // wawit for messages
		MessageReceiveStatus status = ReceiveMessage(buf, buflen);
		if (status != MESSAGE_RECEIVE_STATUS_SUCCESSFUL) continue;

        // read messages
		MessageHeader *header = (MessageHeader*) buf;
		TimeDriverHeader *timeheader = (TimeDriverHeader*) MESSAGE_CONTENT(buf);

        // send response
		if (timeheader->command == GET_TIME)
		{
			TimeDriverCall response;
			response.second  = var.second;
			response.minute  = var.minute;
			response.hour	 = var.hour;
			response.day	 = var.day;
			response.month	 = var.month;
			response.year	 = var.year;

			SendMessageT(header->sender, &response, sizeof(TimeDriverCall), header->transaction);
		}

        // send response
        else if (timeheader->command == GET_UPTIME)
        {
            TimeDriverUptime response;
            response.second = var.upSecond;

            SendMessageT(header->sender, &response, sizeof(TimeDriverUptime), header->transaction);
        }

        // unknown command, probably deprecated or bad
		else Utils::log("uknown command: %i from task: %i", timeheader->command, header->sender);
	}

	delete EvaTime;
}
