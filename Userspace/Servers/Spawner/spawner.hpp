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

#include <eva.h>
#include <string>

#ifndef __SPAWNER__
#define __SPAWNER__

/**
 *
 */
enum BinaryFormat
{
	BF_UNKNOWN,
	BF_ELF32
};

/**
 * create the environment file with prefixed enviroment variables
 */
void createOsEnvironmentFile(const char *dir, const char *fileName);

/**
 * read the inittab.sh and create the initial environment file
 */
void init();

/**
 * Receives incoming message requests.
 */
void receiveRequests();

/**
 *
 */
void protocolError(std::string msg, ...);

/**
 * Reads the arguments from the given pipe.
 *
 * @param request
 * 		incoming message
 * @param requester
 * 		tid of requester
 * @param tx
 * 		transaction to respond on
 */
void processSpawnRequest(SpawnCommandSpawnRequest *request, Tid requester, MessageTransaction tx);

/**
 * Creates the standard input & output streams for the process
 * with the id <pid>. Writes the result to the output parameters
 *
 * @param createdPid
 * 		target process id
 * @param requesterPid
 * 		process id of the requesting task
 * @param outInw
 * 		write end of stdin of the process
 * @param outOutr
 * 		read end of stdout of the process
 * @param outErr
 * 		read end of stderr of the process
 */
bool setupStdio(Pid createdPid, Pid requesterPid, File_t *outInw, File_t *outOutr, File_t *outErr, File_t inStdin, File_t inStdout, File_t inStderr);

/**
 * Places the given command line arguments in the kernel buffer
 * for the target process <targetProc>.
 *
 * @param targetProc
 * 		target process identifier
 * @param args
 * 		arguments to pass
 */
void writeCliArgs(ProcessCreationIdentifier targetProc, const char *args);

/**
 *
 */
BinaryFormat detectFormat(File_t file);

/**
 * Spawns the binary at <path> passing the arguments <args>. As security level,
 * the level <sec_lvl> is set. The out parameters are filled with the respective
 * values on success.
 *
 * @param path
 * 		binary path
 * @param args
 * 		command line arguments
 * @param workdir
 * 		target working directory
 * @param sec_lvl
 * 		security level
 * @param requesterPid
 * 		process id of the requester
 */
SpawnStatus spawn(const char *path, const char *args, const char *workdir, SecurityLevel sec_lvl, Pid requesterPid, Pid *outPid, File_t *outStdin, File_t *outStdout, File_t *outStderr, File_t inStdin, File_t inStdout, File_t inStderr);

#endif
