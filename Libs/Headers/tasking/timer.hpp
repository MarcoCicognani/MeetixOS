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

#ifndef MEETIX_LIBRARY_TASKING_TIMER
#define MEETIX_LIBRARY_TASKING_TIMER

#include <functional>
#include <string>
#include <tasking/thread.hpp>
#include <Utils/TimeSpan.hh>

/**
 * typedefs the functor
 */
using Functor_t = std::function<void()>;

/**
 * permit to create a thread
 * that execute a set of istructions timed bu a timespan
 */
class Timer {
public:
    /**
     * constructor
     *
     * @param tm:			timing of the timer
     * @param function:		the body of the timed thread
     * @param-opt begin:	start execution at instantiation
     */
    Timer(const Utils::TimeSpan& tm, Functor_t func, bool begin = false);

    /**
     * destructor
     */
    ~Timer();

    /**
     * start the timed thread
     * to do only one time because this function
     * creates a new thread each time is called
     *
     * @return whether the operation success, return false if
     * 		   thread is already created or if there was a problem
     */
    bool start();

    /**
     * stop the timed thread
     */
    void stop();

    /**
     * restart a previous stopped timer
     */
    void restart();

private:
    /**
     * internal data
     */
    Tid       timedThreadID; // the id of the created thread
    uint64_t  timing;        // timing for th thread
    uint8_t   lock;          // lock for thread locking
    Functor_t function;      // the body of the thread
};

#endif
