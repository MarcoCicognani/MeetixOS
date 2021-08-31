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

#ifndef EVA_FILESYSTEM_PIPES
#define EVA_FILESYSTEM_PIPES

#include "eva/stdint.h"
#include "eva/kernel.h"
#include "utils/ListEntry.hpp"
#include "tasking/process.hpp"
#include "filesystem/pipes.hpp"

/**
 *
 */
typedef int PipeID;

/**
 *
 */
struct Pipe
{
	uint8_t *buffer;
	uint8_t *write;
	uint8_t *read;
	uint32_t size;
	uint32_t capacity;

	ListEntry<Pid> *references;
};

/**
 *
 */
class Pipes
{
public:

	/**
	 *
	 */
	static void initialize();

	/**
	 *
	 */
	static Pipe *get(PipeID id);

	/**
	 *
	 */
	static PipeID create();

	/**
	 *
	 */
	static void addReference(PipeID pipe, Pid pid);

	/**
	 *
	 */
	static void removeReference(PipeID pipe, Pid pid);

	/**
	 *
	 */
	static bool hasReferenceFromOtherProcess(Pipe *pipe, Pid pid);

};

#endif
