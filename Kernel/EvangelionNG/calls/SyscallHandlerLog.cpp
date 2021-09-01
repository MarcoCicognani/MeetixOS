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

#include <calls/SyscallHandler.hpp>
#include <logger/logger.hpp>
#include <tasking/process.hpp>

/**
 * Writes a message to the system log.
 */
SYSCALL_HANDLER(log)
{
	Process *process = currentThread->process;
	SyscallLog *data = (SyscallLog*) SYSCALL_DATA(currentThread->cpuState);

	// % signs are not permitted, because the internal logger would get confused.
	uint32_t len = String::length(data->message);
	for (uint32_t i = 0; i < len; i++)
		if (data->message[i] == '%') data->message[i] = '!';

	const char *taskIdent = currentThread->getIdentifier();
	if (!taskIdent) taskIdent = currentThread->process->main->getIdentifier();

	// If the task has an identifier, do log with name:
	const char *prefix = "*";
	if (taskIdent != 0) {
		logInfo("%! (%i:%i) %s", taskIdent, process->main->id, currentThread->id, data->message);
	}

	else {
		logInfo("%! (%i:%i) %s", prefix, process->main->id, currentThread->id, data->message);
	}

	return currentThread;
}

/**
 * Sets the log output to the screen enabled or disabled.
 */
SYSCALL_HANDLER(setVideoLog)
{
	SyscallSetVideoLog *data = (SyscallSetVideoLog*) SYSCALL_DATA(currentThread->cpuState);
	Logger::setVideo(data->enabled);
	return currentThread;
}
