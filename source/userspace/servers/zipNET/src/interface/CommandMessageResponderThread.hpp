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

#ifndef __INTERFACE_COMMAND_MESSAGE_RESPONDER_THREAD__
#define __INTERFACE_COMMAND_MESSAGE_RESPONDER_THREAD__

#include <tasking/thread.hpp>
#include <deque>

/**
 *
 */
typedef struct
{
	Tid target;
	uint32_t transaction;
	void *message;
	size_t length;
} CommandMessageResponse_t;

/**
 *
 */
class CommandMessageResponderThread_t : public Thread
{
public:
	std::deque<CommandMessageResponse_t> buffer;
	uint8_t bufferEmpty = true;

	/**
	 *
	 */
	virtual void run();

	/**
	 *
	 */
	void sendResponse(CommandMessageResponse_t &response);

};

#endif
