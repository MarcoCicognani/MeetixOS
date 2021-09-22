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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <Api.h>
#include <tasking/thread.hpp>

/**
 * execute the provided thread instance
 *
 * @param thread:	the thread to be executed
 */
void Thread::execute(Thread* thread) {
    thread->run();
}

/**
 * starts the thread defined with the body
 * on the run method and returns the id of the new thread
 *
 * @return the id of new thread
 */
Tid Thread::start() {
    id = s_create_thread_d((void*)&execute, this);
    return id;
}
