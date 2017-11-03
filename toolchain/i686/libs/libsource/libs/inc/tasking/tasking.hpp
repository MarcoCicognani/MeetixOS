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

#ifndef MEETIX_LIBRARY_TASKING_TASKING
#define MEETIX_LIBRARY_TASKING_TASKING

#include <eva.h>
#include <eva/utils/llist.hpp>

/*
 * static class to get and manage tasks of kernel
 */
class Tasking
{
public:
	/**
	 * @return a list with tids of threads
	 */
	static llist<Tid> getThreadIDs();

	/**
	 * @return a list with pids of Processes
	 */
	static llist<Pid> getProcessIDs();

	/**
	 * @return list with descriptors of process
	 */
	static llist<ProcessDescriptor> getProcess();

	/**
	 * @return list with descriptors of threads
	 */
	static llist<ThreadDescriptor> getThreads();
};

#endif
